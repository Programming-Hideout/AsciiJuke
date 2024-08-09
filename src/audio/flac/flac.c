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
    uint8_t *data_ptr = (uint8_t *)data;
    si->minBlockSize = (data_ptr[0] << 4) | (data_ptr[1] >> 4);
    si->maxBlockSize = ((data_ptr[1] & 0x0f) << 8) | data_ptr[2];
    si->minFrameSize = (data_ptr[3] << 16) | (data_ptr[4] << 8) | data_ptr[5];
    si->maxFrameSize = (data_ptr[6] << 16) | (data_ptr[7] << 8) | data_ptr[8];
    si->sampleRate = (data_ptr[9] << 12) | (data_ptr[10] << 4) | (data_ptr[11] >> 4);
    si->numChannels = ((data_ptr[11] & 0x0f) >> 1);
    si->bitsPerSample = ((data_ptr[11] & 0x01) << 4) | (data_ptr[12] >> 4);
    si->totalSamples = ((data_ptr[12] & 0x0f) << 36) | (data_ptr[13] << 28) | (data_ptr[14] << 20) |
                       (data_ptr[15] << 12) | (data_ptr[16] << 4) | (data_ptr[17] >> 4);
    si->md5sum[0] = ((data_ptr[17] & 0x0f) << 60) | (data_ptr[18] << 52) | (data_ptr[19] << 44) |
                    (data_ptr[20] << 36) | (data_ptr[21] << 28) | (data_ptr[22] << 20) |
                    (data_ptr[23] << 12) | (data_ptr[24] << 4) | (data_ptr[25] >> 4);
    si->md5sum[1] = ((data_ptr[25] & 0x0f) << 60) | (data_ptr[26] << 52) | (data_ptr[27] << 44) |
                    (data_ptr[28] << 36) | (data_ptr[29] <<1 );
*/

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
    si->minBlockSize = swap_uint16(data_ptr[byteOffset] << 8 | data_ptr[byteOffset + 1]);
    byteOffset += 2; // 2 bytes were managed
    si->maxBlockSize = swap_uint16(data_ptr[byteOffset] << 8 | data_ptr[byteOffset + 1]);
    byteOffset += 2;
    if (si->minBlockSize == si->maxBlockSize) {
        si->fixedBlockSize = true;
    }

    // FIXME: {{{
    // This code may not work and we may need to run external scripts to verify
    // Which I will do but I am telling you lot that this code, after this point,
    // was written with uncertainty, hopes, and prayers.
    // This code supports Heisenbergs Uncertainty Principle
    u24Storage = //   4                              5                                 6
        (data_ptr[byteOffset] << 16) | (data_ptr[byteOffset + 1] << 8) | (data_ptr[byteOffset + 2] << 0);
    u24Storage = swap_uint32(u24Storage) >> 8;
    si->minFrameSize = u24Storage;
    si->knownMinFrameSize = !!u24Storage; // If its 0 it is implied the value is not know
    byteOffset += 3;

    u24Storage = //   7                              8                                 9
        (data_ptr[byteOffset] << 16) | (data_ptr[byteOffset + 1] << 8) | (data_ptr[byteOffset + 2] << 0);
    u24Storage = swap_uint32(u24Storage) >> 8;
    si->maxFrameSize = u24Storage;
    si->knownMinFrameSize = !!u24Storage;
    byteOffset += 3;

    u24Storage =
        (data_ptr[byteOffset] << 16) | (data_ptr[byteOffset + 1] << 8) | (data_ptr[byteOffset + 2] << 0);
    u24Storage >>= 4; // we made it to haskell :cheer:
    u24Storage = swap_uint32(u24Storage) >> 12;
    ERROR((u24Storage != 0), "invalid sample rate");
    si->sampleRate = u24Storage;
    byteOffset += 2; // now this is shit. We extract 20 bits, leaving 4

    si->numChannels = data_ptr[byteOffset] >> 5 & 0x7;
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
    // FIXME: }}}
    printf("%ld\n", byteOffset);

    for (int i = byteOffset; i < 34; ++i) {
        printf("%02x ", data_ptr[i]);
    }
    printf("\n");
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
    printf("%.4s\n", ff->flacString);

    // fseek(in, -1, SEEK_CUR);
    uint32_t head_uint;
    metadataBlock_t mdBlock = {};
    void *dataContainer;
    do {
        fread(&head_uint, sizeof(uint32_t), 1, in);
        head_uint = swap_uint32(head_uint);
        __flac_mh_extract(&mdBlock.head, head_uint);
        printf("isLastBlock: %d\nblockType: %d\nblockSize: %d\n", mdBlock.head.isLastBlock,
               mdBlock.head.blockType, mdBlock.head.blockSize);
        mdBlock.data.size = &mdBlock.head.blockSize;
        dataContainer = malloc(mdBlock.head.blockSize);
        fread(dataContainer, sizeof(uint8_t), mdBlock.head.blockSize, in);
        __flac_blocktype_extract(&mdBlock, dataContainer);
        // fseek(in, mdBlock.head.blockSize, SEEK_CUR);
    } while (mdBlock.head.isLastBlock == NOT_LAST_BLOCK);
}
