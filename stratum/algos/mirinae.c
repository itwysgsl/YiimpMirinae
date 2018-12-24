// Copyright (c) 2018 iamstenman
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#include "../sha3/sph_groestl.h"
#include "kupyna/kupyna512.h"
#include "mirinae.h"
#include <memory.h>

/*
#include <chain.h>
#include <microbitcoin.h>
#include <validation.h>

int GetBlockHeight(const CBlockHeader &block) {
    return GetBlockHeight(block.hashPrevBlock);
}

int GetBlockHeight(const uint256 &hashPrevBlock) {
    CBlockIndex *pindexPrev = NULL;
    int nHeight = 0;

    BlockMap::iterator mi = mapBlockIndex.find(hashPrevBlock);
    if (mi != mapBlockIndex.end()) {
        pindexPrev = (*mi).second;
        nHeight = pindexPrev->nHeight + 1;
    }

    return nHeight;
}
*/

// need SEED

//void mirinae(const void* data, size_t length, void* output, int height, const void* seed)
void mirinae_hash(const char* input, char* output, uint32_t len)
{
	void *in = (void*) input;
        int size = 80;

	//seed

	unsigned char hash1[64];
	unsigned char hash2[64];

	//unsigned char hash[64] = { 0 };
	//unsigned char offset[64] = { 0 };
	//const int window = 4096;
	//const int aperture = 32;
	//int64_t n = 0;

	sph_groestl512_context ctx_groestl;
	struct kupyna512_ctx_t ctx_kupyna;
	//kupyna512_context ctx_kupyna;

	//kupyna512_init(&ctx_kupyna);
	//kupyna512_update(&ctx_kupyna, seed, 32); /* seed -> ? */
	//kupyna512_final(&ctx_kupyna, offset);

	//memcpy(&n, offset, 8);

	sph_groestl512_init(&ctx_groestl);
	sph_groestl512(&ctx_groestl, in, size); /* data -> input */
	sph_groestl512_close(&ctx_groestl, hash1);

	/*
	unsigned int h_loop = hash[0];

	for (int i = 0; i < (((n % nHeight) + (height + 1)) % window); i++) {
		for (int j = 0; j < (h_loop % aperture); j++) {
	*/
			kupyna512_init(&ctx_kupyna);
			kupyna512_update(&ctx_kupyna, hash1, 64);
			kupyna512_final(&ctx_kupyna, hash2);
	/*
		}

		h_loop = hash2[0];
	}
	*/

	sph_groestl512_init(&ctx_groestl);
	sph_groestl512(&ctx_groestl, hash2, 64);
	sph_groestl512_close(&ctx_groestl, hash1);

	memcpy(output, hash1, 32);
}
