#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "audio/flac/flac.h"
#include "audio/streaming.h"
#include "audio/wav/wav.h"
#include "portaudio.h"

#define ERROR(cond, msg) assert((printf("%s", (!cond) ? msg : "\0"), cond))

int main(int argc, char **argv) {
    // // printf("Hello, World!");
    // flacStream_t *ff = malloc(sizeof(flacStream_t));
    // FILE *file = fopen("./src/audio/flac/example1.flac", "rb");
    // ERROR(file, "file open failed\n");

    // /* int n = 1;
    // if (*(char *)&n == 1) {
    //     printf("little endian\n");
    // } */
    // flac_read(ff, file);

    // fclose(file);
    // return 0;

    assert(argc > 1);

    FILE *file = fopen(argv[1], "r");

    bool is_valid = is_valid_wav_file(file);
    assert(is_valid && "It seems that the .wav file is corrupted");

    wavHeader_t wav_header = wav_extract_header(file);
    
    printf("\nWav header:\n");
    wav_print_header(wav_header);

    fseek(file, sizeof(wavHeader_t), 0);

    PaError err = initialize_portaudio();
    assert(err == 0 && "Unable to initialize portaudio");

    uint8_t *data = malloc(wav_header.dataSubchunkSize);

    wav_read_all_frames(data, wav_header, file);

    PaStream *pa_stream;
    portaudio_open_default_stream(pa_stream, 2, wav_header.sampleRate, 256, data);

    err = Pa_StartStream(pa_stream);
    print_pa_error(err);
    assert(err == 0 && "Unable to start portaudio stream");
    
    while(Pa_IsStreamActive(pa_stream) == 1) {
        Pa_Sleep(1000);
        printf("Streaming music...\n");
    }

    free(data);
    terminate_portaudio();

    // i guess that i'm causing a memory leak by not deallocating data
}
