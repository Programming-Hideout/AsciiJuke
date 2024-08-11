#include "flac.h"
#include "../../utils/endians.h"
#include "../../utils/errorMacro.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
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
*/

/*
int main(void) {
    flacStream_t ff;
    FILE *in = fopen("./example1.flac", "rb");
    flac_read(&ff, in);
    fclose(in);
    return 0;
}
// */

void flac_view_streaminfo(bt_streamInfo_t *si) {
    printf("╭───╼ %p\n", si);
    printf("├╼ minBlockSize\t\t╻ (uint16_t)\t→ %u\n", si->minBlockSize);
    printf("├╼ maxBlockSize\t\t╏ (uint16_t)\t→ %u\n", si->maxBlockSize);
    printf("├╼ fixedBlockSize\t╏ (bool)\t→ %u\n", si->fixedBlockSize);
    printf("├╼ minFrameSize\t\t╏ (uint32_t)\t→ %u\n", si->minFrameSize);
    printf("├╼ knownMinFrameSize\t╏ (bool)\t→ %u\n", si->knownMinFrameSize);
    printf("├╼ maxFrameSize\t\t╏ (uint32_t)\t→ %u\n", si->maxFrameSize);
    printf("├╼ knownMaxFrameSize\t╏ (bool)\t→ %u\n", si->knownMaxFrameSize);
    printf("├╼ sampleRate\t\t╏ (uint32_t)\t→ %u\n", si->sampleRate);
    printf("\n");
    printf("├╼ numChannels\t\t╏ (uint8_t)\t→ %u\n", si->numChannels);
    printf("├╼ bitsPerSample\t╏ (uint8_t)\t→ %u\n", si->bitsPerSample);
    printf("├╼ totalSamples\t\t╏ (uint64_t)\t→ %lu\n", si->totalSamples);
    printf("╰╼ md5sum[2]\t\t╹ (uint64_t)\t→ %lu%lu\n", si->md5sum[0], si->md5sum[1]);
}

// LINK:
// https://stackoverflow.com/questions/2182002/how-to-convert-big-endian-to-little-endian-in-c-without-using-library-functions

void __flac_mh_extract(metadataBlockHeader_t *mh, uint32_t data) {
    // The first bit is the isLastBlock
    mh->isLastBlock = (lastBlockFlag_e)((data >> 31) & 1);
    // The next 7 bits are the blockType
    mh->blockType = (blockTypes_e)((data >> 24) & 0x7f);
    // The last 24 bits are the blockSize
    mh->blockSize = (data >> 0) & 0xffffff;
}
void __flac_blocktype_streaminfo_extract(bt_streamInfo_t *si, void *data) {
    uint8_t *data_ptr = (uint8_t *)data;
    uint32_t u24Storage = 0; // some ints are not multiples of 8
    uint64_t byteOffset = 0; // for easier code copy pasting and no hardcoding

    // everything is treated as a byte so to convert from byte to uint i concat
    si->minBlockSize = (data_ptr[byteOffset] << 8) | (data_ptr[byteOffset + 1]);
    byteOffset += 2; // 2 bytes were managed
    si->maxBlockSize = (data_ptr[byteOffset] << 8) | (data_ptr[byteOffset + 1]);
    byteOffset += 2;
    if (si->minBlockSize == si->maxBlockSize) {
        si->fixedBlockSize = true;
    }

    u24Storage = //   4                              5                                 6
        (data_ptr[byteOffset] << 16) | (data_ptr[byteOffset + 1] << 8) | (data_ptr[byteOffset + 2] << 0);
    si->minFrameSize = u24Storage;
    si->knownMinFrameSize = !!u24Storage; // If its 0 it is implied the value is not know
    byteOffset += 3;

    u24Storage = //   7                              8                                 9
        (data_ptr[byteOffset] << 16) | (data_ptr[byteOffset + 1] << 8) | (data_ptr[byteOffset + 2] << 0);
    si->maxFrameSize = u24Storage;
    si->knownMaxFrameSize = !!u24Storage;
    byteOffset += 3;

    u24Storage = //  10                             11                                12
        (data_ptr[byteOffset] << 16) | (data_ptr[byteOffset + 1] << 8) | (data_ptr[byteOffset + 2] << 0);
    u24Storage >>= 4; // we made it to haskell :cheer:
    ERROR((u24Storage != 0), "invalid sample rate");
    si->sampleRate = u24Storage;
    printf("%02x %02x %02x %02x\n", data_ptr[byteOffset], data_ptr[byteOffset + 1], data_ptr[byteOffset + 2],
           data_ptr[byteOffset + 3]);
    byteOffset += 2; // now this is shit. We extract 20 bits, leaving 4

    // ([bo][bo][bo][bo] [nc][nc][nc] [ps])([ps][ps][ps][ps] [_][_][_][_])

    // FIXME: {{{
    // This code may not work and we may need to run external scripts to verify
    uint16_t extraction = ((data_ptr[byteOffset] << 8)) | data_ptr[byteOffset + 1];
    printf("\n%u\n", (extraction >> 1) & ((1 << 5) - 1));
    for (int i = 0; i <= 16; ++i) {
        uint16_t e = (extraction >> i) & ((1 << 3) - 1);
        printf("%u:\t %02x\t%u\t%0b\n", i, e, e, e);
    }
    si->numChannels = (data_ptr[byteOffset] >> 0) & 0x7;
    si->bitsPerSample = data_ptr[byteOffset] & 0x1f;
    // we are at 12 now bits now so we can increment byteOffset
    byteOffset += 1;

    // storage size is 36 bits. First 4 bits then bitshift
    si->totalSamples = ((data_ptr[byteOffset] & 0xf) << 24) | (data_ptr[byteOffset + 1] << 16) |
                       (data_ptr[byteOffset] << 8) | (data_ptr[byteOffset] << 0);
    byteOffset += 5;

    si->md5sum[0] = data_ptr[0];
    si->md5sum[1] = data_ptr[1];
    byteOffset += 16;
    flac_view_streaminfo(si);
    // FIXME: }}}
}

void __flac_blocktype_extract(metadataBlock_t *mb, void *data) {
    switch (mb->head.blockType) {
    case STREAMINFO:
        mb->data.data = malloc(sizeof(bt_streamInfo_t));
        __flac_blocktype_streaminfo_extract(mb->data.data, data);
        break;
    case PADDING:
        break;
    case APPLICATION:
        break;
    case SEEKTABLE:
        break;
    case VORBIS_COMMENT:
        break;
    case CUESHEET:
        break;
    case PICTURE:
        break;
    default:
        break;
    }
}

void flac_read(flacStream_t *ff, FILE *in) {
    // Read the first 4 bytes of the file
    fseek(in, 0, SEEK_SET);
    fread(ff->flacString, sizeof(char), FLAC_STR_SIZE, in);

    // fseek(in, -1, SEEK_CUR);
    uint32_t head_uint;
    metadataBlock_t mdBlock = {};
    void *dataContainer;
    do {
        fread(&head_uint, sizeof(uint32_t), 1, in);
        head_uint = swap_uint32(head_uint);
        __flac_mh_extract(&mdBlock.head, head_uint);
        mdBlock.data.size = &mdBlock.head.blockSize;
        dataContainer = malloc(mdBlock.head.blockSize);
        fread(dataContainer, sizeof(uint8_t), mdBlock.head.blockSize, in);
        __flac_blocktype_extract(&mdBlock, dataContainer);
        // fseek(in, mdBlock.head.blockSize, SEEK_CUR);
    } while (mdBlock.head.isLastBlock == NOT_LAST_BLOCK);
}
