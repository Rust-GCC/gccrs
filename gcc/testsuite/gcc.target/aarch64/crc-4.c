/* { dg-do compile } */
/* { dg-additional-options "-O3 -march=armv8-a+crypto -fdump-tree-crc -std=c99" } */

#include <stdint.h>
#include <stddef.h>

uint32_t crc32(const uint8_t *data, size_t length)
{
    uint8_t i;
    uint32_t crc = 0xffffffff; // Initial value
    while (length--) {
        crc ^= (uint32_t)(*data++) << 24;
#pragma GCC unroll 0
        for (i = 0; i < 8; ++i) {
            if (crc & 0x80000000)
                crc = (crc << 1) ^ 0x04C11DB7;
            else
                crc <<= 1;
        }
    }
    return crc;
}

/* { dg-final { scan-tree-dump-times {\.CRC } 1 "crc" } } */
