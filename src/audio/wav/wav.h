#ifndef WAV_H
#define WAV_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "miniaudio.h"

typedef struct wavHeader {
    // beware: the string doesn't end until there's a null char
    // so it's going to read more than than necessary
    unsigned char riff[4];
    uint32_t chunkSize;
    unsigned char fileType[4];

    // format sub-chunk
    unsigned char formatSubchunkMarker[4]; // should be "fmt "
    uint32_t formatSubchunkSize;
    uint16_t audioFormat;
    uint16_t channelCount;
    uint32_t sampleRate;
    uint32_t byteRate;        // bitsPerSample/8 * sampleRate
    uint16_t sampleBlockSize; // bitsPerSample/8 * channelCount
    uint16_t bitsPerSample;

    // data sub-chunk
    unsigned char dataSubchunkMarker[4]; // should be "data"
    uint32_t dataSubchunkSize;
} wavHeader_t;

typedef struct wavData {
    wavHeader_t wavHeader;
    uint8_t* data;
} wavData_t;

wavHeader_t wav_extract_header(uint8_t *stream);
wavHeader_t wav_extract_header_from_file(FILE *stream);
bool is_valid_wav_header(wavHeader_t wav_header);
bool is_valid_wav_file(FILE *stream);

void wav_read_all_frames(uint8_t *buffer, wavHeader_t wav_header, FILE *stream);
void wav_read_next_frame(uint8_t *buffer, wavHeader_t wav_header, FILE *stream);
void wav_read_n_frames(uint8_t *buffer, wavHeader_t wav_header, uint8_t n_frames, FILE *stream);

/**
 * Miniaudio implementation
 */

// static ma_result wav_ma_decoding_backend_init(void *pUserData, ma_read_proc onRead, ma_seek_proc onSeek,
//                                               ma_tell_proc onTell, void *pReadSeekTellUserData,
//                                               const ma_decoding_backend_config *pConfig,
//                                               const ma_allocation_callbacks *pAllocationCallbacks,
//                                               ma_data_source **ppBackend);

// static ma_result wav_ma_decoding_backend_init_file(void *user_data, const char *file_path,
//                                                    const ma_decoding_backend_config *pConfig,
//                                                    const ma_allocation_callbacks *pAllocationCallbacks,
//                                                    ma_data_source **ppBackend);

// static void wav_ma_decoding_backend_uninit(void *pUserData, ma_data_source *pBackend,
//                                            const ma_allocation_callbacks *pAllocationCallbacks);

// static ma_decoding_backend_vtable wav_ma_decoding_backend_vtable = {
//     wav_ma_decoding_backend_init, wav_ma_decoding_backend_init_file, NULL, /* onInitFileW() */
//     NULL,                                                                  /* onInitMemory() */
//     wav_ma_decoding_backend_uninit};

/**
 * DEBUG
 */

#ifdef DEBUG

void wav_print_header(wavHeader_t wav_header);

#endif // DEBUG

#endif // WAV_H