#include "wav_datasource.h"
#include "wav.h"

static ma_result wav_data_source_read(ma_data_source* pDataSource, void* pFramesOut, ma_uint64 frameCount, ma_uint64* pFramesRead)
{
    // Read data here. Output in the same format returned by my_data_source_get_data_format().
}

static ma_result wav_data_source_seek(ma_data_source* pDataSource, ma_uint64 frameIndex)
{
    // Seek to a specific PCM frame here. Return MA_NOT_IMPLEMENTED if seeking is not supported.
    wavData_t* wav_data = (wavData_t*) pDataSource;

    if(frameIndex > (wav_data->wavHeader.dataSubchunkSize/wav_data->wavHeader.chunkSize)) {
        return MA_INVALID_OPERATION;
    }

    return MA_SUCCESS;
}

static ma_result wav_data_source_get_data_format(ma_data_source* pDataSource, ma_format* pFormat, ma_uint32* pChannels, ma_uint32* pSampleRate, ma_channel* pChannelMap, size_t channelMapCap)
{
    wavHeader_t wav_header = wav_extract_header(pDataSource);
    (*pFormat) = ma_format_u8;
    (*pChannelMap) = wav_header.channelCount;
    (*pSampleRate) = wav_header.sampleRate;
    (*pChannels) = wav_header.dataSubchunkSize; //TODO: idk what this is

    return MA_SUCCESS;

}

static ma_result wav_data_source_get_cursor(ma_data_source* pDataSource, ma_uint64* pCursor)
{
    // Retrieve the current position of the cursor here. Return MA_NOT_IMPLEMENTED and set *pCursor to 0 if there is no notion of a cursor.
}

static ma_result wav_data_source_get_length(ma_data_source* pDataSource, ma_uint64* pLength)
{
    // Retrieve the length in PCM frames here. Return MA_NOT_IMPLEMENTED and set *pLength to 0 if there is no notion of a length or if the length is unknown.
    wavData_t* wav_data = (wavData_t*) pDataSource;
    (*pLength) = wav_data->wavHeader.dataSubchunkSize/wav_data->wavHeader.chunkSize;

    return MA_SUCCESS;
}

static ma_data_source_vtable wav_data_source_vtable = {
    wav_data_source_read, wav_data_source_seek, wav_data_source_get_data_format, wav_data_source_get_cursor,
    wav_data_source_get_length};