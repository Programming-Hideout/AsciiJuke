/**
 * I'm following Linux coding style conventions
 * https://gist.github.com/davidzchen/9187984
 */

#ifndef WAV_H
#define WAV_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

struct wav_header {
    char riff_marker[4]; // should be equal to "RIFF"
    uint32_t riff_file_size;
    char wave_marker[4]; // should be equal to "WAVE"

    char format_marker[4];         // should be equal to "fmt "
    uint32_t format_subchunk_size; // should be 16 for PCM
    uint16_t audio_format;         // should be 1 for PCM
    uint16_t channels_count;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t sample_size;
    uint16_t bits_per_sample;

    char data_marker[4]; // should be equal to "data"
    uint32_t data_subchunk_size;
};

// TODO: maybe this one should be part of an unified error interface
enum wav_reading_result {
    SUCCESS,
    INVALID_FILE_POINTER,
    INVALID_FILE_SIZE,
    INVALID_FILE_HEADER,
    UNKNOWN_ERROR
};

struct wav_reader {
    struct wav_header header;
    FILE *file_p;
};

bool wav_is_valid_header(struct wav_header header);

enum wav_reading_result wav_create_reader(struct wav_reader *reader,
                                          FILE *file_p);

void wav_read_frames(struct wav_reader *reader, size_t frames_count,
                     uint8_t *result_buffer);

#endif