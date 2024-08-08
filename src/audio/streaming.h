#include "portaudio.h"
#include <stdint.h>

#ifndef STREAMING_H
#define STREAMING_H

PaError initialize_portaudio();
PaError close_portaudio();

static int portaudio_callback(const void *input_buffer, void *output_buffer, unsigned long frames_per_buffer,
                              const PaStreamCallbackTimeInfo *time_info, PaStreamCallbackFlags status_flags,
                              void *user_data);

PaError portaudio_open_default_stream(PaStream *stream, uint8_t audio_channels_count, uint32_t sample_rate,
                                      uint16_t frames_per_buffer, uint8_t *data);

#endif // STREAMING_H