#ifndef _RC4_H
#define _RC4_H 1

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
"C" {
#endif

typedef struct rc4_state {
    uint8_t sbox[256];
    uint8_t idx1;
    uint8_t idx2;
} RC4_State;

void rc4_reset(RC4_State *state, const uint8_t *key, size_t len);
void rc4_crypt(RC4_State *state, const uint8_t *in, uint8_t *out, size_t len);

#ifdef __cplusplus
}
#endif

#endif
