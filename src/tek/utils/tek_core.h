#ifndef _TEK_CORE_H
#define _TEK_CORE_H

#include "tek_types.h"
#include "../platform/tek_keys.h"
#include "tek_stopwatch.h"
#include "tek_text.h"
#include <string.h>

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ZERO_MEM_VAR(var) memset(&var, 0, sizeof(var))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#endif
