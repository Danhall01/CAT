#ifndef _INCLUDE_CAT_MEMORY_H_
#define _INCLUDE_CAT_MEMORY_H_

#include "internal/functions.h"

#include <stddef.h>
#include <stdint.h>

CAT_API void catRaiiFree(void* ptr);

#if !__has_attribute(cleanup)
    #error \
        "The `cleanup` attribute is not supported with current compiler. This is used throughout the library"
#else
    #define raii raiiA(catRaiiFree)
    #define raiiA(fn) __attribute__((cleanup(fn)))
#endif

#endif /* _INCLUDE_CAT_MEMORY_H_ */
