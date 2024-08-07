#ifndef WAV_H
#define WAV_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


typedef struct wavHeader {
    // beware: the string doesn't end until there's a null char
    // so it's going to read more than than necessary
    unsigned char chunkId[4];
    uint32_t chunkSize;
    unsigned char format[4];

    // format sub-chunk
    unsigned char formatSubchunkId[4]; // should be "fmt "
    uint32_t formatSubchunkSize;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate; // bitsPerSample/8 * sampleRate
    uint16_t blockAllign;
    uint16_t bitsPerSample;

    // data sub-chunk
    unsigned char dataSubchunkId[4]; // should be "data"
    uint32_t dataSubchunkSize;
} wavHeader_t;

wavHeader_t wav_extract_header(FILE *in);
bool is_valid_wav_file(FILE *in);

void wav_read_entire_data_chunk(uint8_t* buffer,wavHeader_t wav_header, FILE *in);
void wav_read_next_sample(uint8_t* buffer, wavHeader_t wav_header, FILE *in);
void wav_read_samples(uint8_t* buffer, wavHeader_t wav_header, uint8_t n_samples, FILE *in);

#endif // WAV_H