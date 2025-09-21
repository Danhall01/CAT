#ifndef _INCLUDE_CAT_DEFER_H_
#define _INCLUDE_CAT_DEFER_H_

#include "internal/macro.h"

#define catDefer(after)                                                                               \
    for (uint32_t CAT_MACRO_VAR(_i) = 0; !CAT_MACRO_VAR(_i); CAT_MACRO_VAR(_i) = ({                \
                                                                 after;                            \
                                                                 1                                 \
                                                             }))


#endif /* _INCLUDE_CAT_DEFER_H_ */
