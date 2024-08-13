#include "wav_datasource.h"
#include "wav.h"
#include <time.h>

static ma_result wav_data_source_read(ma_data_source *pDataSource,
                                      void *pFramesOut, ma_uint64 frameCount,
                                      ma_uint64 *pFramesRead) {
    // Read data here. Output in the same format returned by
    // my_data_source_get_data_format().

    wav_data_t *data = (wav_data_t*)pDataSource;
    pFramesOut = data->data; // uint8_t* -> void*
}

static ma_result wav_data_source_seek(ma_data_source *pDataSource,
                                      ma_uint64 frameIndex) {
    // Seek to a specific PCM frame here. Return MA_NOT_IMPLEMENTED if seeking
    // is not supported.
}

static ma_result
wav_data_source_get_data_format(ma_data_source *pDataSource, ma_format *pFormat,
                                ma_uint32 *pChannels, ma_uint32 *pSampleRate,
                                ma_channel *pChannelMap, size_t channelMapCap) {
    // Return the format of the data here.
    wav_data_t *data = (wav_data_t *)pDataSource;
    if (!wav_is_valid_header(data->header)) {
        return MA_INVALID_DATA;
    }

    (*pFormat) = ma_format_u8;
    pChannels = NULL;
    (*pSampleRate) = data->header.sample_rate;
    (*pChannelMap) = data->header.channels_count;

    return MA_SUCCESS;
}

static ma_result wav_data_source_get_cursor(ma_data_source *pDataSource,
                                            ma_uint64 *pCursor) {
    // Retrieve the current position of the cursor here. Return
    // MA_NOT_IMPLEMENTED and set *pCursor to 0 if there is no notion of a
    // cursor.
}

static ma_result wav_data_source_get_length(ma_data_source *pDataSource,
                                            ma_uint64 *pLength) {
    // Retrieve the length in PCM frames here. Return MA_NOT_IMPLEMENTED and set
    // *pLength to 0 if there is no notion of a length or if the length is
    // unknown.
}