
#ifndef MP3_H
#define MP3_H
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>


typedef struct {
  uint8_t version;
  uint8_t layer;
  uint8_t bitrateIndex;
  uint8_t samplingRateIndex;
  uint8_t channelMode;
} MP3;

 void handleErr(const char *err_msg);

 int getBitrate(uint8_t version, uint8_t layer, uint8_t bitrateIndex);

 int getSamplingRate(uint8_t version, uint8_t samplingRateIndex);
#endif
