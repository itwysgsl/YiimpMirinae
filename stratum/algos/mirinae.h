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

void mirinae_hash(const void* input, void* output, size_t length, int height, const void* seed);

#ifdef __cplusplus
}
#endif

#endif
