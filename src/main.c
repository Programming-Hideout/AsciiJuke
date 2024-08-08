#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "audio/flac/flac.h"
#include "audio/wav/wav.h"

#define ERROR(cond, msg) assert((printf("%s", (!cond) ? msg : "\0"), cond))

int main(int argc,char **argv) {
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

    wav_print_header(wav_header);
}
