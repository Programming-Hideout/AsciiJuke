#include "streaming.h"
#include "portaudio.h"
#include <stdint.h>

PaError initialize_portaudio() { return Pa_Initialize(); }
PaError terminate_portaudio() { return Pa_Terminate(); }

// TODO: delete this
typedef struct streamData {
    int16_t right_phase;
    int16_t left_phase;
} streamData_t;

static int portaudio_callback(__attribute__((unused)) const void *input_buffer, void *output_buffer, unsigned long frames_per_buffer,
                              __attribute__((unused)) const PaStreamCallbackTimeInfo *time_info, __attribute__((unused)) PaStreamCallbackFlags status_flags,
                              void *user_data) {

    streamData_t *data = (streamData_t *)user_data; // TODO: make this one's size variable somehow

    int16_t* out = (int16_t*)output_buffer;

    for(uint16_t i=0; i<frames_per_buffer; i++ )
    {
         (*out++) = data->left_phase;  /* left */
         (*out++) = data->right_phase;  /* right */
        /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
        data->left_phase += 0.01f;
        /* When signal reaches top, drop back down. */
        if( data->left_phase >= 1.0f ) data->left_phase -= 2.0f;
        /* higher pitch so we can distinguish left and right. */
        data->right_phase += 0.03f;
        if( data->right_phase >= 1.0f ) data->right_phase -= 2.0f;
    }
    return 0;

}

PaError portaudio_open_default_stream(PaStream *stream, uint8_t audio_channels_count, uint32_t sample_rate,
                                      uint16_t frames_per_buffer, uint8_t *data) {
    return Pa_OpenDefaultStream(&stream, 0, audio_channels_count, paInt16, sample_rate, frames_per_buffer,
                                portaudio_callback, &data);
}