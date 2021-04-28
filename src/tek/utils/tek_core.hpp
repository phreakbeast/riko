#ifndef _TEK_CORE_HPP
#define _TEK_CORE_HPP

#include "tek_types.hpp"
#include "../platform/tek_keys.hpp"
#include "tek_stopwatch.hpp"
#include "tek_text.hpp"
#include <string.h>

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ZERO_MEM_VAR(var) memset(&var, 0, sizeof(var))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#endif
