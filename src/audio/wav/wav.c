#include "wav.h"

bool wav_is_valid_header(wav_header_t header) {
    if (header.riff_marker[0] != 'R' || header.riff_marker[1] != 'I' ||
        header.riff_marker[2] != 'F' || header.riff_marker[3] != 'F') {
        return false;
    }

    if (header.wave_marker[0] != 'W' || header.wave_marker[1] != 'A' ||
        header.wave_marker[2] != 'V' || header.wave_marker[3] != 'E') {
        return false;
    }

    if (header.format_marker[0] != 'f' || header.format_marker[1] != 'm' ||
        header.format_marker[2] != 't' || header.format_marker[3] != ' ') {
        return false;
    }

    if (header.data_marker[0] != 'd' || header.data_marker[1] != 'a' ||
        header.data_marker[2] != 't' || header.data_marker[3] != 'a') {
        return false;
    }

    // Right now we're only supporting PCM
    if (header.audio_format != 1 || header.audio_format != 16) {
        return false;
    }

    return true;
}