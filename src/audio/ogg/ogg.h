#ifndef OGG_H
#define OGG_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define HEADER_SIZE 27

typedef struct oggHeader {
    uint32_t capturePattern;
    uint8_t version;
    uint8_t headerType;
    uint64_t granulePosition;
    uint32_t bitstreamSerialNumber;
    uint32_t pageSequenceNumber;
    uint32_t checksum;
    uint8_t pageSegments;
} oggHeader_t;

/*
 * The identification header is a short header of only a few fields used to declare the stream definitively as Vorbis,
 * and provide a few externally relevant pieces of information about the audio stream.
 */ 
typedef struct oggVorbisInfo {
    uint32_t vorbisVersion;
    uint8_t audioChannels;
    uint32_t sampleRate;
    uint32_t bitrateMaximum;
    uint32_t bitrateNominal;
    uint32_t bitrateMinimum;
    void *codecSetup;
} oggVorbisInfo_t;

/*
 * Big structure that will contail most of metadata about vorbis pages and packets.
 */
typedef struct stb_vorbis
{
    // user-accessible info
    unsigned int sample_rate;
    int channels;
    // memory allocation
    unsigned int setup_memory_required;
    unsigned int temp_memory_required;
    unsigned int setup_temp_memory_required;
    // vorbis comments
    char *vendor;
    int comment_list_length;
    char **comment_list;

    // file name and offsets
    FILE *f;
    uint32_t f_start;
    int close_on_free;

    // file size
    uint8_t *stream;
    uint8_t *stream_start;
    uint8_t *stream_end;
    uint32_t stream_len;
    uint8_t  push_mode;

    // the page to seek to when seeking to start, may be zero
    uint32_t first_audio_page_offset;

    // header info
    int blocksize[2];
    int blocksize_0, blocksize_1;
    int codebook_count;
    uint32_t total_samples;

    // current page/packet/segment streaming info
    uint32_t serial; // stream serial number for verification
    int last_page;
    int segment_count;
    uint8_t segments[255];
    uint8_t page_flag;
    uint8_t bytes_in_seg;
    uint8_t first_decode;
    int next_seg;
    int last_seg;  // flag that we're on the last segment
    int last_seg_which; // what was the segment number of the last seg?
    uint32_t acc;
    int valid_bits;
    int packet_bytes;
    int end_seg_with_known_loc;
    uint32_t known_loc_for_packet;
    int discard_samples_deferred;
    uint32_t samples_output;

    // sample-access
    int channel_buffer_start;
    int channel_buffer_end;
} stb_vorbis;

// Function definitions
oggHeader_t ogg_extract_header(FILE *in);
bool is_valid_ogg_file(FILE *in);
static int read_n_bytes(FILE *in, uint8_t *data, int n);

#endif  /* OGG_H */