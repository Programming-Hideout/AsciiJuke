#ifndef STREAMING_H
#define STREAMING_H


#include "miniaudio.h"

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

#endif // STREAMING_H