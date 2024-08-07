#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "audio/flac/flac.h"
#define ERROR(cond, msg) assert((printf("%s", (!cond) ? msg : "\0"), cond))

int main() {
    // printf("Hello, World!");
    flacStream_t *ff = malloc(sizeof(flacStream_t));
    FILE *file = fopen("./src/audio/flac/example1.flac", "rb");
    ERROR(file, "file open failed\n");

    /* int n = 1;
    if (*(char *)&n == 1) {
        printf("little endian\n");
    } */
    flac_read(ff, file);

    fclose(file);
    return 0;
}
