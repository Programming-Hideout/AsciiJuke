#include "flac.h"

#include <stdint.h>

void flac_init(flacFile_t *ff) {
    for (uint8_t flacStrIdx = 0; flacStrIdx < 4; flacStrIdx++) {
        ff->flacString[flacStrIdx] = FLAC_STR[flacStrIdx];
    }
}
