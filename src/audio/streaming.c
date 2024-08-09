#include "streaming.h"
#include "portaudio.h"
#include <stdint.h>
#include <stdio.h>

PaError initialize_portaudio() { return Pa_Initialize(); }
PaError terminate_portaudio() { return Pa_Terminate(); }

// TODO: delete this
typedef struct streamData {
    int16_t right_phase;
    int16_t left_phase;
} streamData_t;

static int portaudio_callback(__attribute__((unused)) const void *input_buffer, void *output_buffer,
                              unsigned long frames_per_buffer,
                              __attribute__((unused)) const PaStreamCallbackTimeInfo *time_info,
                              __attribute__((unused)) PaStreamCallbackFlags status_flags, void *user_data) {

    streamData_t *data = (streamData_t *)user_data; // TODO: make this one's size variable somehow

    int16_t *out = (int16_t *)output_buffer;

    for (uint16_t i = 0; i < frames_per_buffer; i++) {
        (*out++) = data->left_phase;  /* left */
        (*out++) = data->right_phase; /* right */
    }
    return 0;
}

PaError portaudio_open_default_stream(PaStream *stream, uint8_t audio_channels_count, uint32_t sample_rate,
                                      uint16_t frames_per_buffer, uint8_t *data) {
    return Pa_OpenDefaultStream(&stream, 0, audio_channels_count, paInt16, sample_rate, frames_per_buffer,
                                portaudio_callback, data);
}

#ifdef DEBUG
void print_pa_error(PaError pa_error) {
    switch (pa_error) {
    case paNoError:
        printf("\nNo Error\n");
        break;
    case paNotInitialized:
        printf("\nPA not initialized\n");
        break;
    case paUnanticipatedHostError:
        printf("\nUnanticipated host error\n");
        break;
    case paInvalidChannelCount:
        printf("\nInvalid channel count\n");
        break;
    case paInvalidSampleRate:
        printf("\nInvalid sample rate\n");
        break;
    case paInvalidDevice:
        printf("\nInvalid device\n");
        break;
    case  paBadStreamPtr:
        printf("\nBad stream pointer\n");
    default:
        printf("\nUnknown error: %i\n", pa_error);
        break;
    }
}
#endif