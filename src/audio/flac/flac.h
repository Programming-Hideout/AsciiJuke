#ifndef FLAC_H
#define FLAC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define FLAC_STRING "fLaC"
#define FLAC_STR_SIZE 4

/* typedef struct audioFrame {
    //
} audioFrame_t; */

typedef struct bt_streamInfo {
    uint16_t minBlockSize;
    uint16_t maxBlockSize;
    bool fixedBlockSize;
    uint32_t minFrameSize;
    bool knownMaxFrameSize;
    uint32_t maxFrameSize;
    bool knownMinFrameSize;
    uint32_t sampleRate;
    uint8_t numChannels;
    uint8_t bitsPerSample;
    uint64_t totalSamples;
    uint64_t md5sum[2];
} bt_streamInfo_t;

typedef enum lastBlockFlag {
    NOT_LAST_BLOCK = 0,
    LAST_BLOCK = 1,
} lastBlockFlag_e;

typedef enum blockTypes {
    STREAMINFO = 0,
    PADDING = 1,
    APPLICATION = 2,
    SEEKTABLE = 3,
    VORBIS_COMMENT = 4,
    CUESHEET = 5,
    PICTURE = 6,
} blockTypes_e;

typedef struct metadataBlockHeader {
    lastBlockFlag_e isLastBlock; // 1 if it is, 0 if not.  1 Bit
    blockTypes_e blockType;      // 0 for streamInfo, etc. 7 Bits
    uint32_t blockSize;          // the size of the metadata block. 24 Bits
} metadataBlockHeader_t;

// Block size refers to the size of the metaDataBlockData
typedef struct metadataBlockData {
    // Technically you dont need this / this isnt stored
    // I will still store it to keep track of the block data size
    uint32_t *size; // pointer for memory efficiency
    void *data;
} metadataBlockData_t;

typedef struct metadataBlock {
    metadataBlockHeader_t head;
    metadataBlockData_t data;
    // TODO: uint32_t xorVal;  // XOR linked list of metadata blocks
    struct metadataBlock *prev;
    struct metadataBlock *next;
} metadataBlock_t;

typedef struct flacStream {
    char flacString[FLAC_STR_SIZE]; // fLaC
    metadataBlock_t mdBlock;
    // linked list so it only needs to be traversed once
    // metadataBlockHeader_t streamInfo;
    // metadataBlock_t *metaData;
    // audioFrame_t audioFrame;
} flacStream_t;

void flac_read(flacStream_t *ff, FILE *in);

#endif // FLAC_H
