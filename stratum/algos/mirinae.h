// Copyright (c) 2018 iamstenman
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MBCHASH_H
#define MBCHASH_H

// #include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

const char* mirinae(const void* input, void* output, uint32_t len, int height, const void* seed);
void mirinae_hash(const char* input, char* output, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
