#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#ifndef FOR_RANGE
  #define FOR_RANGE(var, from, to, jumps) for (int var = (from); var < (to); var+=jumps)
#endif

#define FOR(var, till) FOR_RANGE(var, 0, till, 1)

#define ASSERT(a, msg) if(a) {printf("ERROR: %s\n", msg); exit(1);}
#define IN_BOUNDRY(n, n1, n2) (n1 <= n && n <= n2)

#define ARRAY_SIZE(s) (sizeof(s)/sizeof(*s))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define IGNORE(a) do { if(a); } while (0)

#ifndef PRINT_VAR
  #define PRINT_VAR(x) _Generic((x), \
	int: printf("%s = %d\n", #x, x),\
	float: printf("%s = %d\n", #x, x),\
	char: printf("%s = %c\n", #x, x),\
	u8: printf("%s = %d\n", #x, x),\
	bool: printf("%s = %d\n", #x, x),\
	short: printf("%s = %d\n", #x, x),\
	size_t: printf("%s = %d\n", #x, x),\
	unsigned int: printf("%s = %u\n", #x, x))
#endif

#ifndef PRINT_STR
  #define PRINT_STR(x) printf("%s = %s\n", #x, x);
#endif

#endif
