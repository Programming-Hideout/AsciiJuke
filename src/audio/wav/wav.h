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
    uint16_t sampleBlockSize; // bitsPerSample/8 * numChannels
    uint16_t bitsPerSample;

    // data sub-chunk
    unsigned char dataSubchunkId[4]; // should be "data"
    uint32_t dataSubchunkSize;
} wavHeader_t;

wavHeader_t wav_extract_header(FILE *stream);
bool is_valid_wav_file(FILE *stream);

void wav_read_all_frames(uint8_t *buffer, wavHeader_t wav_header, FILE *stream);
void wav_read_next_frame(uint8_t *buffer, wavHeader_t wav_header, FILE *stream);
void wav_read_n_frames(uint8_t *buffer, wavHeader_t wav_header, uint8_t n_frames, FILE *stream);

void wav_stereo_split_channels(uint8_t *rightch_buffer, uint8_t *leftch_buffer, uint8_t *original_buffer, wavHeader_t wav_header);

#ifndef DEBUG
#define DEBUG
#endif

#ifdef DEBUG

void wav_print_header(wavHeader_t wav_header);

#endif // DEBUG

#endif // WAV_H