#include "flac.h"
#include "../../utils/endians.h"

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

void flac_mh_extract(metadataBlockHeader_t *mh, uint32_t data) {
    // The first bit is the isLastBlock
    mh->isLastBlock = (lastBlockFlag_e)((data >> 31) & 1);
    // The next 7 bits are the blockType
    mh->blockType = (blockTypes_e)((data >> 24) & 0x7f);
    // The last 24 bits are the blockSize
    mh->blockSize = (data >> 0) & 0xffffff;
}
void flac_blocktype_streaminfo_extract(bt_streamInfo_t *si, void *data) {
    printf("\n");

    uint8_t *data_ptr = (uint8_t *)data;

    si->minBlockSize = swap_uint16(data_ptr[0] << 8 | data_ptr[1]);
    printf("minBlockSize: %d\n", si->minBlockSize);
    si->maxBlockSize = swap_uint16(data_ptr[2] << 8 | data_ptr[3]);
    printf("maxBlockSize: %d\n", si->maxBlockSize);

    printf("\n");
}
void flac_blocktype_extract(metadataBlock_t *mb, void *data) {
    switch (mb->head.blockType) {
    case STREAMINFO:
        mb->data.data = malloc(sizeof(bt_streamInfo_t));
        flac_blocktype_streaminfo_extract(mb->data.data, mb->data.data);
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
        flac_mh_extract(&mdBlock.head, head_uint);
        printf("isLastBlock: %d\nblockType: %d\nblockSize: %d\n", mdBlock.head.isLastBlock,
               mdBlock.head.blockType, mdBlock.head.blockSize);
        mdBlock.data.size = &mdBlock.head.blockSize;
        dataContainer = malloc(mdBlock.head.blockSize);
        fread(dataContainer, sizeof(uint8_t), mdBlock.head.blockSize, in);
        flac_blocktype_extract(&mdBlock, dataContainer);
        // fseek(in, mdBlock.head.blockSize, SEEK_CUR);
    } while (mdBlock.head.isLastBlock == NOT_LAST_BLOCK);
}
