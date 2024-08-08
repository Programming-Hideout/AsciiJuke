#include "portaudio.h"

#ifndef STREAMING_H
#define STREAMING_H

PaError initialize_portaudio();

static int portaudio_callback(const void *input_buffer, void *output_buffer, unsigned long frames_per_buffer,
                              const PaStreamCallbackTimeInfo *time_info, PaStreamCallbackFlags status_flags,
                              void *user_data);

#endif // STREAMING_H