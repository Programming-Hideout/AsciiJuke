#include "ogg.h"

oggPageHeader_t ogg_extract_header(FILE *in) {
    oggPageHeader_t header;
    fread(&header, HEADER_SIZE, 1, in);
    // Print parsed header information
    print_page_header(&header);
    // Getting the offset of next oggs page header
    uint8_t *segmentTable = malloc(header.segments);
    fread(segmentTable, 1, header.segments, in);
    uintptr_t nextOffset = (ftell(in) + *segmentTable);
    // TODO: Getting the size of data
    return header;
}

bool is_valid_ogg_file(FILE *in) {
    const char *signature = "OggS"; // OGG Vorbis compressed audio files must have a signature (tag) OggS (hex: 4F 67 67 53) at the beginning of the audiofile.
    uint32_t magicNumbers;
    if (fread(&magicNumbers, sizeof(magicNumbers), 1, in) != sizeof(magicNumbers)) {
        return false;
    }

    if (memcmp(signature, &magicNumbers, sizeof(magicNumbers)) == 0) {
        return true;
    }
    return false;
}

static int read_n_bytes(FILE *in, uint8_t *data, int n) {
    if (fread(data, n, 1, in) == n) {
        return 1;
    }
    else {
        return 0;
    }
}

void print_page_header(oggPageHeader_t *pageHeader) {
    printf("Ogg Page Information:\n");
    printf("Version: %d\n", pageHeader->streamVersion);
    printf("Header Type: %d\n", pageHeader->typeFlag);
    printf("Granule Position: %llu\n", (unsigned long long)pageHeader->granulePosition);
    printf("Serial Number: %u\n", pageHeader->streamID);
    printf("Sequence Number: %u\n", pageHeader->pageNumber);
    printf("Checksum: %u\n", pageHeader->checksum);
    printf("Segments: %d\n", pageHeader->segments);
}
