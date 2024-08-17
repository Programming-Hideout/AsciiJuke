#include "mp3.h"
#include <stdint.h>

void handleErr(const char *err_msg) {
  fprintf(stderr, "%s: %s\n", err_msg, strerror(errno));
}

// Function to get bitrate from index
int getBitrate(uint8_t version, uint8_t layer, uint8_t bitrateIndex) {

  static const int bitrateTable[2][3][16] = {
      {
          // MPEG Version 2 & 2.5
          {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256,
           0}, // Layer III
          {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256,
           0}, // Layer II
          {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320,
           0} // Layer I
      },
      {
          // MPEG Version 1
          {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320,
           0}, // Layer III
          {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384,
           0}, // Layer II
          {0, 32, 64, 96, 128, 160, 192, 224, 256, 320, 384, 448, 512, 576, 640,
           0} // Layer I
      }};
  return bitrateTable[version == 3][3 - layer][bitrateIndex] * 1000;
}
//get sample rate from index
int getSamplingRate(uint8_t version, uint8_t samplingRateIndex) {
  // add error handle
  static const int samplingRateTable[4][3] = {
      {11025, 12000, 8000},  // MPEG Version 2.5
      {0, 0, 0},             // Reserved
      {22050, 24000, 16000}, // MPEG Version 2
      {44100, 48000, 32000}  // MPEG Version 1
  };
  return samplingRateTable[version][samplingRateIndex];
}
