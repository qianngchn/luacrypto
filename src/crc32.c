#include "crc32.h"

#define CRC32_POLY 0xEDB88320U

void crc32_init(CRC32_Handle *handle, uint32_t init) {
    size_t i = 0, j = 0;

    for (i = 0; i < 256; i++) {
        handle->table[i] = i;
        for (j = 0; j < 8; j++) {
            if (handle->table[i] & 0x00000001U)
                handle->table[i] = (handle->table[i] >> 1) ^ CRC32_POLY;
            else
                handle->table[i] = handle->table[i] >> 1;
        }
    }

    handle->crc32 = handle->init = init;
}

void crc32_update(CRC32_Handle *handle, const uint8_t *buf, size_t len) {
    handle->crc32 ^= 0xFFFFFFFFU;

    while (len--)
        handle->crc32 = (handle->crc32 >> 8) ^ handle->table[(uint8_t)(handle->crc32) ^ *buf++];

    handle->crc32 ^= 0xFFFFFFFFU;
}
