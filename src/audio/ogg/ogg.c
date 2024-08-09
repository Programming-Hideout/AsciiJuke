#include "ogg.h"

oggHeader_t ogg_extract_header(FILE *in) {
    oggHeader_t header;
    fread(&header, HEADER_SIZE, 1, in);
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
