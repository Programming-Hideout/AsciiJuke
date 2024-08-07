
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef struct wavHeader {
        // beware: the string doesn't end until there's a null char
        // so it may read more than necessary
        unsigned char chunkId[4];
        uint32_t chunkSize;
        unsigned char format[4];

        // format sub-chunk
        unsigned char formatSubchunkId[4]; // should be "fmt "
        uint32_t formatSubchunkSize;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAllign;
        uint16_t bitsPerSample;

        // data sub-chunk
        unsigned char dataSubchunkId[4]; // should be "data"
        uint32_t dataSubchunkSize;
} wavHeader_t;
