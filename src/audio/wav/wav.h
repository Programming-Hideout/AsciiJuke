/**
 * I'm following Linux coding style conventions
 * https://gist.github.com/davidzchen/9187984
 */

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char riff_marker[4]; // should be equal to "RIFF"
    uint32_t riff_file_size;
    char wave_marker[4]; // should be equal to "WAVE"

    char format_marker[4]; // should be equal to "fmt "
    uint32_t format_subchunk_size; // should be 16 for PCM
    uint16_t audio_format; // should be 1 for PCM
    uint16_t channels_count;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t sample_size;
    uint16_t bits_per_sample;

    char data_marker[4]; // should be equal to "data"
    uint32_t data_subchunk_size;

} wav_header_t;

typedef struct {
   wav_header_t header; // header data
   uint8_t* data; // audio data
} wav_data_t;

bool wav_is_valid_header(wav_header_t header);