#include <stdio.h>

#include "flac/flac.h"

int main() {
    // printf("Hello, World!");
    flacStream_t *ff;
    FILE *file = fopen("./src/flac/example1.flac", "rb");

    /* int n = 1;
    if (*(char *)&n == 1) {
        printf("little endian\n");
    } */
    flac_read(ff, file);

    fclose(file);
    return 0;
}
