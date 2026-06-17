/* { dg-do run } */
/* { dg-options "-O2 -minline-all-stringops" } */

/* Test -O2 -minline-all-stringops on memcpy with various bounds.  */

#include <stdlib.h>

#define MAX_LENGTH 4096
#define EXTRA 64
#define TOTAL (EXTRA + MAX_LENGTH + EXTRA)

#define MEMCPY_BOUND(BOUND) \
__attribute__ ((noipa, noinline)) \
static char * \
memcpy_##BOUND (char *dest, char *src, size_t len) \
{ \
  if (len <= BOUND) \
    return __builtin_memcpy (dest + EXTRA, src, len); \
  else \
    return dest + EXTRA; \
}

#define CHECK_MEMCPY_BOUND(BOUND, SIZE) \
  { \
    for (i = 0; i < TOTAL; i++) \
      { \
	dest[i] = 'a'; \
	src[i] = 'A'; \
      } \
    p = memcpy_##BOUND (dest, src, SIZE); \
    if (p != dest + EXTRA) \
      abort (); \
    for (i = 0; i < SIZE; i++, p++) \
      if (*p != 'A') \
        abort (); \
    for (; i < (TOTAL - EXTRA); i++, p++) \
      if (*p != 'a') \
	abort (); \
    p = dest; \
    for (i = 0; i < EXTRA; i++, p++) \
      if (*p != 'a') \
	abort (); \
  }

#define CHECK_MEMCPY(SIZE) \
  CHECK_MEMCPY_BOUND (SIZE, SIZE) \
  if (SIZE > 1) \
    CHECK_MEMCPY_BOUND (SIZE, SIZE - 1) \
  if (SIZE > 2) \
    CHECK_MEMCPY_BOUND (SIZE, SIZE - 2) \
  if (SIZE > 3) \
    CHECK_MEMCPY_BOUND (SIZE, SIZE - 3) \
  if (SIZE > 4) \
    CHECK_MEMCPY_BOUND (SIZE, SIZE - 4) \
  if (SIZE > 5) \
    CHECK_MEMCPY_BOUND (SIZE, SIZE - 5) \
  if (SIZE > 6) \
    CHECK_MEMCPY_BOUND (SIZE, SIZE - 6) \
  if (SIZE > 7) \
    CHECK_MEMCPY_BOUND (SIZE, SIZE - 7)

char dest[TOTAL];
char src[TOTAL];

MEMCPY_BOUND (0);
MEMCPY_BOUND (1);
MEMCPY_BOUND (2);
MEMCPY_BOUND (3);
MEMCPY_BOUND (4);
MEMCPY_BOUND (5);
MEMCPY_BOUND (7);
MEMCPY_BOUND (8);
MEMCPY_BOUND (9);
MEMCPY_BOUND (15);
MEMCPY_BOUND (16);
MEMCPY_BOUND (17);
MEMCPY_BOUND (31);
MEMCPY_BOUND (32);
MEMCPY_BOUND (33);
MEMCPY_BOUND (63);
MEMCPY_BOUND (64);
MEMCPY_BOUND (65);
MEMCPY_BOUND (127);
MEMCPY_BOUND (128);
MEMCPY_BOUND (129);
MEMCPY_BOUND (255);
MEMCPY_BOUND (256);
MEMCPY_BOUND (257);

int
main (void)
{
  unsigned int i;
  char *p;

  CHECK_MEMCPY (0);
  CHECK_MEMCPY (1);
  CHECK_MEMCPY (2);
  CHECK_MEMCPY (3);
  CHECK_MEMCPY (4);
  CHECK_MEMCPY (5);
  CHECK_MEMCPY (7);
  CHECK_MEMCPY (8);
  CHECK_MEMCPY (9);
  CHECK_MEMCPY (15);
  CHECK_MEMCPY (16);
  CHECK_MEMCPY (17);
  CHECK_MEMCPY (31);
  CHECK_MEMCPY (32);
  CHECK_MEMCPY (33);
  CHECK_MEMCPY (63);
  CHECK_MEMCPY (64);
  CHECK_MEMCPY (65);
  CHECK_MEMCPY (127);
  CHECK_MEMCPY (128);
  CHECK_MEMCPY (129);
  CHECK_MEMCPY (255);
  CHECK_MEMCPY (256);
  CHECK_MEMCPY (257);

  return 0;
}
