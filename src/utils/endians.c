#include "endians.h"
#include <stdint.h>

uint16_t swap_uint16(uint16_t val) { return (val << 8) | (val >> 8); }
uint32_t swap_uint32(uint32_t val) {
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}
uint64_t unsignedSwapEndian(uint64_t data, uint8_t numBytes) {
    uint64_t value = 0;
    for (uint8_t byteIdx = 0; byteIdx < numBytes; ++byteIdx) {
        value <<= 1;
        value += (uint8_t)((char *)(&data))[byteIdx];
    }
    return value;
}
