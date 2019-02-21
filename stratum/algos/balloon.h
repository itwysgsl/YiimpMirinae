#ifndef BALLOON_H
#define BALLOON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void balloon_hash(const char* input, char* output, uint32_t height);

#ifdef __cplusplus
}
#endif

#endif
