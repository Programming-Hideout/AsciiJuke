#include "flac.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* typedef struct streamInfo {
    uint64_t minBlockSize;
    uint64_t maxBlockSize;
    uint64_t minFrameSize;
    uint64_t maxFrameSize;
    uint64_t sampleRate;
    uint64_t numChannels;
    uint64_t bitsPerSample;
    uint64_t totalSamples;
    uint8_t md5sum[16];
} streamInfo_t;

typedef struct metadataBlock {
    uint8_t lastBlock;
    uint8_t blockType;
    uint8_t *data;
} metadataBlock_t;

typedef struct audioFrame {
    uint8_t syncCode[14];
    uint8_t reserved;
    uint8_t blockingStrategy;
    uint8_t blockSize;
    uint8_t sampleRate;
    uint8_t channelAssignment;
    uint8_t sampleSize;
    uint8_t sampleNumber;
    uint8_t crc8;
} audioFrame_t;


    // char metadataBlockHeader[4];
    // void * application;
    void *padding;
    // char *vorbisComment;
    // char *cueSheet;
    // char *picture;
*/

// LINK:
// https://stackoverflow.com/questions/2182002/how-to-convert-big-endian-to-little-endian-in-c-without-using-library-functions
uint16_t swap_uint16(uint16_t val) { return (val << 8) | (val >> 8); }
uint32_t swap_uint32(uint32_t val) {
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

void flac_mh_extract(metadataBlockHeader_t *mh, uint32_t data) {
    // The first bit is the isLastBlock
    mh->isLastBlock = (lastBlockFlag_e)((data >> 31) & 1);
    // The next 7 bits are the blockType
    mh->blockType = (blockTypes_e)((data >> 24) & 0x7f);
    // The last 24 bits are the blockSize
    mh->blockSize = (data >> 0) & 0xffffff;
}

void flac_read(flacStream_t *ff, FILE *in) {
    // Read the first 4 bytes of the file
    fseek(in, 0, SEEK_SET);
    fread(ff->flacString, sizeof(char), FLAC_STR_SIZE, in);
    printf("%.4s\n", ff->flacString);

    // fseek(in, -1, SEEK_CUR);
    uint32_t head_uint;
    metadataBlock_t mdBlock = {};
    // metadataBlockHeader_t header_data;
    do {
        fread(&head_uint, sizeof(uint32_t), 1, in);
        head_uint = swap_uint32(head_uint);
        flac_mh_extract(&mdBlock.head, head_uint);
        printf("isLastBlock: %d\nblockType: %d\nblockSize: %d\n",
               mdBlock.head.isLastBlock, mdBlock.head.blockType,
               mdBlock.head.blockSize);
        mdBlock.data.size = &mdBlock.head.blockSize;
        mdBlock.data.data = malloc(mdBlock.head.blockSize);
        fread(mdBlock.data.data, sizeof(uint8_t), mdBlock.head.blockSize, in);
        // fseek(in, mdBlock.head.blockSize, SEEK_CUR);
    } while (mdBlock.head.isLastBlock == 0);
}