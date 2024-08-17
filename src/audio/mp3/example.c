#include "mp3.h"

// Function to parse the MP3 frame header
int parseFrame(uint8_t *header, MP3 *frm) {
  uint16_t syncWord = (header[0] << 3) | (header[1] >> 5);
  if (syncWord != 0x7FF) {
    handleErr("invalid frame header\n");
    return 1;
  }

  frm -> version = (header[1] >> 3) & 0x3;
  frm -> layer = (header[1] >> 1) & 0x3;
  frm -> bitrateIndex = (header[2] >> 4) & 0xF;
  frm -> samplingRateIndex = (header[2] >> 2) & 0x3;
  frm -> channelMode = (header[3] >> 6) & 0x3;

  if (frm -> layer == 0 || frm -> bitrateIndex == 0xF ||frm -> samplingRateIndex == 0x3) {
    handleErr("Invalid frame parameters\n");
    return 1;
  }

  const char *versionStr[4] = {"MPEG Version 2.5", "Reserved", "MPEG Version 2",
                              "MPEG Version 1"};
  const char *layerStr[4] = {"Reserved", "Layer III", "Layer II", "Layer I"};
  const char *channelModeStr[4] = {"Stereo", "Joint Stereo", "Dual Channel",
                                  "Mono"};

  int bitrate = getBitrate(frm -> version,  frm -> layer, frm -> bitrateIndex);
  int samplingRate = getSamplingRate(frm -> version, frm -> samplingRateIndex);

  printf("Frame detected:\n");
  printf("  Version: %s\n", versionStr[frm -> version]);
  printf("  Layer: %s\n", layerStr[frm -> layer]);
  printf("  Bitrate: %d kbps\n", bitrate / 1000);
  printf("  Sampling Rate: %d Hz\n", samplingRate);
  printf("  Channel Mode: %s\n", channelModeStr[frm -> channelMode]);
  return 0;
}

void readMp3(FILE *fl) {
  uint8_t buffer[4];
  size_t bytesRead;

  bytesRead = fread(buffer, 1, sizeof(buffer), fl);
}

int main(void) {
  uint8_t buffer[4];
  size_t bytesRead;

  MP3 frame;

  FILE *file = fopen("sample2.mp3", "rb");

  if (!file) {
   handleErr("unable to open file");
    return 1;
  }

  // Read the first 4 bytes from the file
  bytesRead = fread(buffer, 1, 4, file);
  if (bytesRead < 4) {
    handleErr("cound't read the first 4 bytes\n");
    fclose(file);
    return 1;
  }

  // Loop through the file to find and parse the first MP3 frame
  while (1) {

    if ((buffer[0] == 0xFF) && ((buffer[1] & 0xE0) == 0xE0)) {
      if (parseFrame(buffer, &frame) != 0) {
        handleErr("error occured hile parsing frame");
        return 1;
      };
      break; // Stop after finding the first valid frame
    }

    // Shift the buffer and read the next byte
    memmove(buffer, buffer + 1, 3);
    bytesRead = fread(&buffer[3], 1, 1, file);
    if (bytesRead < 1) {
      printf("error occured in fread\n");
      break;
      return 1;
    }
  }

  fclose(file);
  return 0;
}
