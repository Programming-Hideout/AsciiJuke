#ifndef OGG_H
#define OGG_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define HEADER_SIZE 27

typedef struct oggHeader {
    uint32_t capturePattern;
    uint8_t version;
    uint8_t headerType;
    uint64_t granulePosition;
    uint32_t bitstreamSerialNumber;
    uint32_t pageSequenceNumber;
    uint32_t checksum;
    uint8_t pageSegments;
} oggHeader_t;

/*
 * The identification header is a short header of only a few fields used to declare the stream definitively as Vorbis,
 * and provide a few externally relevant pieces of information about the audio stream.
 */ 
typedef struct oggVorbisInfo {
    uint32_t vorbisVersion;
    uint8_t audioChannels;
    uint32_t sampleRate;
    uint32_t bitrateMaximum;
    uint32_t bitrateNominal;
    uint32_t bitrateMinimum;
    void *codecSetup;
} oggVorbisInfo_t;

oggHeader_t ogg_extract_header(FILE *in);
bool is_valid_ogg_file(FILE *in);

#endif  /* OGG_H */