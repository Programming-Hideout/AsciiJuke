#include "streaming.h"
#include "portaudio.h"
#include <stdint.h>

PaError initialize_portaudio() { return Pa_Initialize(); }

static int portaudio_callback(const void *input_buffer, void *output_buffer, unsigned long frames_per_buffer,
                              const PaStreamCallbackTimeInfo *time_info, PaStreamCallbackFlags status_flags,
                              void *user_data) {

    uint8_t *data = (uint8_t *)user_data;

    // TODO
    return 0;
}