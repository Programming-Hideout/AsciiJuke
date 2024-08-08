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

oggHeader_t ogg_extract_header(FILE *in);
bool is_valid_ogg_file(FILE *in);

#endif  /* OGG_H */