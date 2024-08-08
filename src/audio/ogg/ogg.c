#include "ogg.h"


oggHeader_t ogg_extract_header(FILE *in) {
    oggHeader_t header;
    fread(&header, HEADER_SIZE, 1, in);
    return header;
}

bool is_valid_ogg_file(FILE *in) {
    const char *signature = "OggS"; // OGG Vorbis compressed audio files must have a signature (tag) OggS (hex: 4F 67 67 53) at the beginning of the audiofile.
    uint32_t magicNumber;
    fread(&magicNumber, sizeof(magicNumber), 1, in);
    if (memcmp(signature, magicNumber, sizeof(magicNumber)) == 0) {
        return true;
    }
    return false;
}

