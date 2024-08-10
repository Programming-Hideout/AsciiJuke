#include "streaming.h"

void data_callback(ma_device* p_device, void* p_output, const void* p_input, ma_uint32 frame_count) {
    ma_decoder* p_decoder = (ma_decoder*)p_device->pUserData;
    if (p_decoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(p_decoder, p_output, frame_count, NULL);

    (void)p_input;
}

