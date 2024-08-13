#include "miniaudio.h"
#include "wav.h"

typedef struct
{
    ma_data_source_base base;
    wav_data_t wav_data;
} wav_data_source_t;

static ma_result wav_data_source_read(ma_data_source *pDataSource, void *pFramesOut, ma_uint64 frameCount,
                                     ma_uint64 *pFramesRead);

static ma_result wav_data_source_seek(ma_data_source *pDataSource, ma_uint64 frameIndex);

static ma_result wav_data_source_get_data_format(ma_data_source *pDataSource, ma_format *pFormat,
                                                ma_uint32 *pChannels, ma_uint32 *pSampleRate,
                                                ma_channel *pChannelMap, size_t channelMapCap);
static ma_result wav_data_source_get_cursor(ma_data_source *pDataSource, ma_uint64 *pCursor);

static ma_result wav_data_source_get_length(ma_data_source *pDataSource, ma_uint64 *pLength);

static ma_data_source_vtable g_wav_data_source_vtable;

ma_result wav_data_source_init(wav_data_source_t* wav_data_source);
void wav_data_source_uninit(wav_data_source_t* wav_data_source);