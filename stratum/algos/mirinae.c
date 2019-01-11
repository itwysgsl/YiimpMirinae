// Copyright (c) 2018 iamstenman
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "sha3/sph_groestl.h"
#include "kupyna/kupyna512.h"
#include "mirinae.h"

void mirinae_hash(const char* input, char* output, uint32_t height)
{
	unsigned char hash[64] = { 0 };
	unsigned char offset[64] = { 0 };
	unsigned char seed[32] = { 0 };
	const int window = 64;
	const int aperture = 32;
	int64_t n = 0;

	sph_groestl512_context ctx_groestl;
	struct kupyna512_ctx_t ctx_kupyna;
	memcpy(seed, input + 4, (36 - 4) * sizeof(*input));

	kupyna512_init(&ctx_kupyna);
	kupyna512_update(&ctx_kupyna, seed, 32);
	kupyna512_final(&ctx_kupyna, offset);
	memcpy(&n, offset, 8);

	sph_groestl512_init(&ctx_groestl);
	sph_groestl512(&ctx_groestl, input, 80);
	sph_groestl512_close(&ctx_groestl, hash);

	unsigned int light = (hash[0] > 0) ? hash[0] : 1;
	for (int i = 0; i < (((n % height) + (height + 1)) % window); i++) {
		unsigned int inner_loop = (light % aperture);
		for (int j = 0; j < inner_loop; j++) {
			kupyna512_init(&ctx_kupyna);
			kupyna512_update(&ctx_kupyna, hash, 64);
			kupyna512_final(&ctx_kupyna, hash);
		}

		light = (hash[inner_loop] > 0) ? hash[inner_loop] : 1;
	}

	sph_groestl512_init(&ctx_groestl);
	sph_groestl512(&ctx_groestl, hash, 64);
	sph_groestl512_close(&ctx_groestl, hash);

	memcpy(output, hash, 32);
}
