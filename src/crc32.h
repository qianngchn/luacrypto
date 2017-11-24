#ifndef _CRC32_H
#define _CRC32_H 1

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
"C" {
#endif

typedef struct crc32_handle {
    uint32_t table[256];
    uint32_t crc32;
    uint32_t init;
} CRC32_Handle;

void crc32_init(CRC32_Handle *handle, uint32_t init);
void crc32_update(CRC32_Handle *handle, const uint8_t *buf, size_t len);

static inline void crc32_fetch(CRC32_Handle *handle, uint32_t *crc32) {
    *crc32 = handle->crc32;
    handle->crc32 = handle->init;
}

#ifdef __cplusplus
}
#endif

#endif
