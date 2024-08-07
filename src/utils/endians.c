#include "endians.h"

uint16_t swap_uint16(uint16_t val) { return (val << 8) | (val >> 8); }
uint32_t swap_uint32(uint32_t val) {
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}
