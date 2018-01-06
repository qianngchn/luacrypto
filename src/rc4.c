#include "rc4.h"

static inline void swap(uint8_t *a, uint8_t *b) { uint8_t c = *a; *a = *b; *b = c; }

void rc4_reset(RC4_STATE *state, const uint8_t *key, size_t len) {
    size_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < 256; i++)
        state->sbox[i] = (uint8_t)i;
    state->idx1 = 0;
    state->idx2 = 0;

    for (i = 0; i < 256; i++) {
        j += state->sbox[i] + key[i % len];
        swap(&state->sbox[i], &state->sbox[j]);
    }
}

void rc4_crypt(RC4_STATE *state, const uint8_t *in, uint8_t *out, size_t len) {
    size_t i = 0;
    uint8_t j = 0;

    for (i = 0; i < len; i++) {
        state->idx1++;
        state->idx2 += state->sbox[state->idx1];
        swap(&state->sbox[state->idx1], &state->sbox[state->idx2]);
        j = state->sbox[state->idx1] + state->sbox[state->idx2];
        out[i] = in[i] ^ state->sbox[j];
    }
}
