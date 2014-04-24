#ifndef __PUZZLE_COMMON_H__
#define __PUZZLE_COMMON_H__ 1


#define STDC_HEADERS
#include <memory.h>
//m
#include <cilk\cilk.h>
#include <cilk/cilk_api.h>
//m
#include <string.h>
//m
#define THRESHOLD (float)0.12
//uncomment to switch on low level parallelization
//#define	LOW_PARALL 
#define DEBUGMODE
#ifdef STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>
#else
# if HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif
#include <stdio.h>
#ifdef HAVE_STRING_H
# if !STDC_HEADERS && HAVE_MEMORY_H
#  include <memory.h>
# endif
# include <string.h>
#else
# if HAVE_STRINGS_H
#  include <string.h>
# endif
#endif
#include <limits.h>
#include <errno.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <sys/types.h>

#ifndef errno
extern int errno;
#endif

#endif
