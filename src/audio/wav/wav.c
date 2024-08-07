#include "wav.h"
#include <stdio.h>
#include <stdlib.h>

wavHeader_t wav_extract_header(FILE *in) {
    rewind(in); // set file pointer to the start of the file

    wavHeader_t wav_header;
    fread(&wav_header, sizeof(wavHeader_t), 1, in);

    return wav_header;
}

// this loads the whole file into memory,
// we need a way to divide this into buffers
uint8_t *wav_read_entire_data_chunk(wavHeader_t wav_header, FILE *in) {
    uint8_t *data = (uint8_t *)malloc(wav_header.dataSubchunkSize);
    fread(data, wav_header.dataSubchunkSize, 1, in);

    return data;
}

// it should ammount to one second of data
uint8_t *wav_read_next_sample(wavHeader_t wav_header, FILE *in) {
    uint8_t *data = (uint8_t *)malloc(wav_header.byteRate);
    fread(data, wav_header.byteRate, 1, in);
    
    return data;
}