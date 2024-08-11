#include "wav.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>


wavHeader_t wav_extract_header(uint8_t *stream) {
    wavHeader_t* wav_header;

    wav_header = (wavHeader_t*) stream;
    return *wav_header;
}

wavHeader_t wav_extract_header_from_file(FILE *stream) {
    assert(stream && "The file pointer should be valid");

    rewind(stream); // set file pointer to the start of the file

    wavHeader_t wav_header;
    fread(&wav_header, sizeof(wavHeader_t), 1, stream);

    return wav_header;
}

bool is_valid_wav_file(FILE *stream) {
    assert(stream && "The file pointer should be valid");

    fseek(stream, 0, SEEK_END);
    unsigned long file_size = ftell(stream);

    if (file_size <= sizeof(wavHeader_t)) {
        return false;
    }

    rewind(stream);

    wavHeader_t wav_header = wav_extract_header(stream);
    if (file_size != (wav_header.chunkSize + 8)) {
        return false;
    }

    // if (wav_header.dataSubchunkSize != (file_size - sizeof(wavHeader_t))) {
    //     return false;
    // }

    return true; // seems legit
}

void wav_read_all_frames(uint8_t *buffer, wavHeader_t wav_header, FILE *stream) {
    assert(stream && "The file pointer should be valid");

    int result = fseek(stream, sizeof(wavHeader_t), 0);

    assert(result == 0);

    fread(buffer, wav_header.dataSubchunkSize, 1, stream);
}

void wav_read_next_frame(uint8_t *buffer, wavHeader_t wav_header, FILE *stream) {
    assert(stream && "The file pointer should be valid");

    fread(buffer, wav_header.sampleBlockSize, 1, stream);
}

void wav_read_n_frames(uint8_t *buffer, wavHeader_t wav_header, uint8_t n_frames, FILE *stream) {
    assert(stream && "The file pointer should be valid");

    fread(buffer, wav_header.sampleBlockSize * n_frames, 1, stream);
}

/**
 * Miniaudio implementation
 */


#ifdef DEBUG

void wav_print_header(wavHeader_t wav_header) {
    printf("File size - 8 bytes: %i\n", wav_header.chunkSize);
    printf("Audio format: %i\n", wav_header.audioFormat);
    printf("Number of channels: %i\n", wav_header.numChannels);
    printf("Sample rate: %iHz\n", wav_header.sampleRate);
    printf("Byte rate: %iBytes/s\n", wav_header.byteRate);
    printf("Sample block size: %i Bytes\n", wav_header.sampleBlockSize);
    printf("Bits per sample: %i bits\n", wav_header.bitsPerSample);
}

#endif