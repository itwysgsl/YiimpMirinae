#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "sha3/sph_groestl.h"
#include "balloon.h"

uint64_t u8tou64(uint8_t const *u8){
	uint64_t u64;
	memcpy(&u64, u8, sizeof(u64));
	return u64;
}

void getsalt(const void* input, unsigned char* salt) {
	memcpy(salt, input + 4, 32 * sizeof(*input));
}

void *block_index(uint8_t *blocks, size_t i) {
	return &blocks[0] + (32 * i);
}
void balloon_hash(const char* input, char* output, uint32_t len)
{
	const uint64_t s_cost = 64;
	const uint64_t t_cost = 2;
	const int delta = 3;
	
	sph_groestl512_context ctx_groestl;
	uint8_t blocks[s_cost * 64];
	uint64_t cnt = 0;

	const int slength = 32;
	uint8_t salt[slength];
	getsalt(input, salt);
	
	sph_groestl512_init(&ctx_groestl);
	sph_groestl512(&ctx_groestl, (uint8_t *)&cnt, sizeof((uint8_t *)&cnt));
	sph_groestl512(&ctx_groestl, input, length);
	sph_groestl512(&ctx_groestl, salt, slength);
	sph_groestl512_close(&ctx_groestl, block_index(blocks, 0));
	cnt++;

	for (int m = 1; m < s_cost; m++) {
		sph_groestl512(&ctx_groestl, (uint8_t *)&cnt, sizeof((uint8_t *)&cnt));
		sph_groestl512(&ctx_groestl, block_index(blocks, m - 1), 64);
		sph_groestl512_close(&ctx_groestl, block_index(blocks, m));
		cnt++;
	}

	for (uint64_t t = 0; t < t_cost; t++) {
		for (uint64_t m = 0; m < s_cost; m++) {
			sph_groestl512(&ctx_groestl, (uint8_t *)&cnt, sizeof((uint8_t *)&cnt));
			sph_groestl512(&ctx_groestl, block_index(blocks, (m - 1) % s_cost), 64);
			sph_groestl512(&ctx_groestl, block_index(blocks, m), 64);
			sph_groestl512_close(&ctx_groestl, block_index(blocks, m));
			cnt++;

			for (uint64_t i = 0; i < delta; i++) {
				uint8_t index[64];
				sph_groestl512(&ctx_groestl, (uint8_t *)&t, sizeof((uint8_t *)&t));
				sph_groestl512(&ctx_groestl, (uint8_t *)&cnt, sizeof((uint8_t *)&cnt));
				sph_groestl512(&ctx_groestl, (uint8_t *)&m, sizeof((uint8_t *)&m));
				sph_groestl512(&ctx_groestl, salt, slength);
				sph_groestl512(&ctx_groestl, (uint8_t *)&i, sizeof((uint8_t *)&i));
				sph_groestl512_close(&ctx_groestl, index);
				cnt++;

				uint64_t other = u8tou64(index) % s_cost;
				cnt++;

				sph_groestl512(&ctx_groestl, (uint8_t *)&cnt, sizeof((uint8_t *)&cnt));
				sph_groestl512(&ctx_groestl, block_index(blocks, m), 64);
				sph_groestl512(&ctx_groestl, block_index(blocks, other), 64);
				sph_groestl512_close(&ctx_groestl, block_index(blocks, m));
				cnt++;
			}
		}
	}
	
	memcpy(output, block_index(blocks, s_cost - 1), 32);
}
