#include "wav.h"

bool wav_is_valid_header(struct wav_header header) {
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

enum wav_reading_result wav_create_reader(struct wav_reader *reader,
                                          FILE *file_p) {
    if (file_p == NULL) {
        return INVALID_FILE_POINTER;
    }

    // I know that the != 0 is redundant, but imo it makes the code more
    // readable
    if (fseek(file_p, 0, SEEK_END) != 0) {
        return UNKNOWN_ERROR;
    }

    size_t file_size = ftell(file_p);

    rewind(file_p);

    if (file_size < sizeof(struct wav_header)) {
        return INVALID_FILE_SIZE;
    }

    struct wav_header header;
    if (!fread(&header, sizeof(struct wav_header), 1, file_p)) {
        return UNKNOWN_ERROR;
    }

    if (!wav_is_valid_header(header)) {
        return INVALID_FILE_HEADER;
    }

    reader->file_p = file_p;
    reader->header = header;
    
    return SUCCESS;
}