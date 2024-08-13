#include <stdio.h>

#define MA_NO_VORBIS    /* Disable the built-in Vorbis decoder to ensure the libvorbis decoder is picked. */
#define MA_NO_OPUS      /* Disable the (not yet implemented) built-in Opus decoder to ensure the libopus decoder is picked. */
#define MINIAUDIO_IMPLEMENTATION

int main() { 
    printf("Hello world\n\n");
}