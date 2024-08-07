#include "wav.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define START_OF_DATA_SUBCHUNK 40

wavHeader_t wav_extract_header(FILE *in) {
    assert(in && "The file pointer should be valid");

    rewind(in); // set file pointer to the start of the file

    wavHeader_t wav_header;
    fread(&wav_header, sizeof(wavHeader_t), 1, in);

    return wav_header;
}

bool is_valid_wav_file(FILE *in) {
    assert(in && "The file pointer should be valid");

    fseek(in, 0, SEEK_END);
    long file_size = ftell(in);

    if (file_size <= sizeof(wavHeader_t)) {
        return false;
    }

    rewind(in);

    wavHeader_t wav_header = wav_extract_header(in);
    if (file_size != (wav_header.chunkSize + 8)) {
        return false;
    }

    if (wav_header.dataSubchunkSize != (file_size - START_OF_DATA_SUBCHUNK)) {
        return false;
    }

    return true; // seems legit
}

// this loads the whole file into memory,
// we need a way to divide this into buffers
void wav_read_entire_data_chunk(uint8_t *buffer, wavHeader_t wav_header,
                                FILE *in) {

    assert(in && "The file pointer should be valid");
    assert((sizeof(buffer) >= wav_header.dataSubchunkSize) &&
           "The buffer size should be enough to contain the data");

    int result = fseek(in, sizeof(wavHeader_t), 0);

    assert(result == 0);

    fread(buffer, wav_header.dataSubchunkSize, 1, in);
}

// it should ammount to one second of data
void wav_read_next_sample(uint8_t *buffer, wavHeader_t wav_header, FILE *in) {

    assert(in && "The file pointer should be valid");
    assert((sizeof(buffer) >= sizeof(wav_header.byteRate)) &&
           "The buffer size should be enough to contain the data");

    fread(buffer, wav_header.byteRate, 1, in);
}

void wav_read_samples(uint8_t *buffer, wavHeader_t wav_header,
                      uint8_t n_samples, FILE *in) {

    assert(in && "The file pointer should be valid");
    assert((sizeof(buffer) >= sizeof(wav_header.byteRate * n_samples)) &&
           "The buffer size should be enough to contain the data");

    fread(buffer, wav_header.byteRate * n_samples, 1, in);
}
