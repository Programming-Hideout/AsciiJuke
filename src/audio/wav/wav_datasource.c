#include "wav_datasource.h"

static ma_result wav_data_source_read(ma_data_source *pDataSource,
                                      void *pFramesOut, ma_uint64 frameCount,
                                      ma_uint64 *pFramesRead) {
    // Read data here. Output in the same format returned by
    // wav_data_source_get_data_format().

    wav_data_source_t *data_source = (wav_data_source_t *)pDataSource;

    pFramesOut = data_source->wav_data.data; // uint8_t* -> void*
    return MA_SUCCESS;
}

static ma_result wav_data_source_seek(ma_data_source *pDataSource,
                                      ma_uint64 frameIndex) {
    // Seek to a specific PCM frame here. Return MA_NOT_IMPLEMENTED if seeking
    // is not supported.
    wav_data_source_t *data_source = (wav_data_source_t *)pDataSource;
    uint32_t data_subchunk_size =
        data_source->wav_data.header.data_subchunk_size;
    uint16_t sample_size = data_source->wav_data.header.sample_size;

    data_source->base.pCurrent = data_source->wav_data.data + frameIndex * data_subchunk_size / sample_size;

    return MA_SUCCESS;
}

static ma_result
wav_data_source_get_data_format(ma_data_source *pDataSource, ma_format *pFormat,
                                ma_uint32 *pChannels, ma_uint32 *pSampleRate,
                                ma_channel *pChannelMap, size_t channelMapCap) {
    // Return the format of the data here.
    wav_data_source_t *data_source = (wav_data_source_t *)pDataSource;
    if (!wav_is_valid_header(data_source->wav_data.header)) {
        return MA_INVALID_DATA;
    }

    (*pFormat) = ma_format_u8;
    pChannels = NULL;
    (*pSampleRate) = data_source->wav_data.header.sample_rate;
    (*pChannelMap) = data_source->wav_data.header.channels_count;

    return MA_SUCCESS;
}

static ma_result wav_data_source_get_cursor(ma_data_source *pDataSource,
                                            ma_uint64 *pCursor) {
    // Retrieve the current position of the cursor here. Return
    // MA_NOT_IMPLEMENTED and set *pCursor to 0 if there is no notion of a
    // cursor.
    pCursor = NULL;
    return MA_NOT_IMPLEMENTED;
}

static ma_result wav_data_source_get_length(ma_data_source *pDataSource,
                                            ma_uint64 *pLength) {
    // Retrieve the length in PCM frames here. Return MA_NOT_IMPLEMENTED and set
    // *pLength to 0 if there is no notion of a length or if the length is
    // unknown.

    wav_data_t *data = (wav_data_t *)pDataSource;
    (*pLength) = data->header.data_subchunk_size;
    return MA_SUCCESS;
}

static ma_data_source_vtable g_wav_data_source_vtable =
{
    wav_data_source_read,
    wav_data_source_seek,
    wav_data_source_get_data_format,
    wav_data_source_get_cursor,
    wav_data_source_get_length
};


ma_result wav_data_source_init(wav_data_source_t* wav_data_source)
{
    ma_result result;
    ma_data_source_config base_config;

    base_config = ma_data_source_config_init();
    base_config.vtable = &g_wav_data_source_vtable;

    result = ma_data_source_init(&base_config, &wav_data_source->base);
    if (result != MA_SUCCESS) {
        return result;
    }

    // ... do the initialization of your custom data source here ...

    return MA_SUCCESS;
}

void wav_data_source_uninit(wav_data_source_t* wav_data_source)
{
    // ... do the uninitialization of your custom data source here ...

    // You must uninitialize the base data source.
    ma_data_source_uninit(&wav_data_source->base);
}