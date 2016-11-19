/* utils.h:
 * Util macros/functions */

#ifndef _KERNEL_UTILS_H
#define _KERNEL_UTILS_H

/* This approach avoids double evaluation */
#define MIN(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#define MAX(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#endif
