#include "wav.h"
#include <stdio.h>

wavHeader_t wav_extract_header(FILE *in) {
    rewind(in); // set file pointer to the start of the file

    wavHeader_t wav_header;
    fread(&wav_header, sizeof(wavHeader_t), 1, in);

    return wav_header;
}