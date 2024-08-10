#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "audio/flac/flac.h"
#include "audio/streaming.h"
#include "audio/wav/wav.h"
#include "miniaudio.h"

#define ERROR(cond, msg) assert((printf("%s", (!cond) ? msg : "\0"), cond))

int main(int argc, char **argv) {

    assert(argc > 1 && "No file argument");

    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    result = ma_decoder_init_file(argv[1], NULL, &decoder);
    assert(result == MA_SUCCESS && "Couldn't initialize the decoder");

    FILE *file = fopen(argv[1], "r");

    bool is_valid = is_valid_wav_file(file);
    assert(is_valid && "It seems that the .wav file is corrupted");
    // TODO
    wavHeader_t wav_header = wav_extract_header(file);
    
    printf("\nWav header:\n");
    wav_print_header(wav_header);

    fseek(file, sizeof(wavHeader_t), 0);


    uint8_t *data = malloc(wav_header.dataSubchunkSize);

    wav_read_all_frames(data, wav_header, file);

    free(data);


    // i guess that i'm causing a memory leak by not deallocating data
}
