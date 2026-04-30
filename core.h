#ifndef core_h_INCLUDED
#define core_h_INCLUDED

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>
#include <dlfcn.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

#define KILOBYTE 1000
#define MEGABYTE 1000000
#define GIGABYTE 1000000000

#include "./assert.h"
#include "./stack.h"
#include "./rect.h"
#include "./random.h"
#include "./math.h"
#include "./fiedler.h"
#include "./dynamic_library.h"

#endif
