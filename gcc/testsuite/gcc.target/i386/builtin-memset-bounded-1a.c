/* { dg-do run } */
/* { dg-options "-O2 -minline-all-stringops" } */

/* Test -O2 -minline-all-stringops on memset with various bounds.  */

#include <stdlib.h>

#define MAX_LENGTH 4096
#define EXTRA 64
#define TOTAL (EXTRA + MAX_LENGTH + EXTRA)

#define MEMSET_BOUND(BOUND) \
__attribute__ ((noipa, noinline)) \
static char * \
memset_##BOUND (char *buf, size_t len) \
{ \
  if (len <= BOUND) \
    return __builtin_memset (buf + EXTRA, 'A', len); \
  else \
    return buf + EXTRA; \
}

#define CHECK_MEMSET_BOUND(BOUND, SIZE) \
  { \
    for (i = 0; i < TOTAL; i++) \
      buf[i] = 'a'; \
    p = memset_##BOUND (buf, SIZE); \
      if (p != buf + EXTRA) \
	abort (); \
    for (i = 0; i < SIZE; i++, p++) \
      if (*p != 'A') \
	abort (); \
    for (; i < (TOTAL - EXTRA); i++, p++) \
      if (*p != 'a') \
	abort (); \
    p = buf; \
    for (i = 0; i < EXTRA; i++, p++) \
      if (*p != 'a') \
	abort (); \
  }

#define CHECK_MEMSET(SIZE) \
  CHECK_MEMSET_BOUND (SIZE, SIZE) \
  if (SIZE > 1) \
    CHECK_MEMSET_BOUND (SIZE, SIZE - 1) \
  if (SIZE > 2) \
    CHECK_MEMSET_BOUND (SIZE, SIZE - 2) \
  if (SIZE > 3) \
    CHECK_MEMSET_BOUND (SIZE, SIZE - 3) \
  if (SIZE > 4) \
    CHECK_MEMSET_BOUND (SIZE, SIZE - 4) \
  if (SIZE > 5) \
    CHECK_MEMSET_BOUND (SIZE, SIZE - 5) \
  if (SIZE > 6) \
    CHECK_MEMSET_BOUND (SIZE, SIZE - 6) \
  if (SIZE > 7) \
    CHECK_MEMSET_BOUND (SIZE, SIZE - 7)

char buf[TOTAL];

MEMSET_BOUND (0);
MEMSET_BOUND (1);
MEMSET_BOUND (2);
MEMSET_BOUND (3);
MEMSET_BOUND (4);
MEMSET_BOUND (5);
MEMSET_BOUND (7);
MEMSET_BOUND (8);
MEMSET_BOUND (9);
MEMSET_BOUND (15);
MEMSET_BOUND (16);
MEMSET_BOUND (17);
MEMSET_BOUND (31);
MEMSET_BOUND (32);
MEMSET_BOUND (33);
MEMSET_BOUND (63);
MEMSET_BOUND (64);
MEMSET_BOUND (65);
MEMSET_BOUND (127);
MEMSET_BOUND (128);
MEMSET_BOUND (129);
MEMSET_BOUND (255);
MEMSET_BOUND (256);
MEMSET_BOUND (257);

int
main (void)
{
  unsigned int i;
  char *p;

  CHECK_MEMSET (0);
  CHECK_MEMSET (1);
  CHECK_MEMSET (2);
  CHECK_MEMSET (3);
  CHECK_MEMSET (4);
  CHECK_MEMSET (5);
  CHECK_MEMSET (7);
  CHECK_MEMSET (8);
  CHECK_MEMSET (9);
  CHECK_MEMSET (15);
  CHECK_MEMSET (16);
  CHECK_MEMSET (17);
  CHECK_MEMSET (31);
  CHECK_MEMSET (32);
  CHECK_MEMSET (33);
  CHECK_MEMSET (63);
  CHECK_MEMSET (64);
  CHECK_MEMSET (65);
  CHECK_MEMSET (127);
  CHECK_MEMSET (128);
  CHECK_MEMSET (129);
  CHECK_MEMSET (255);
  CHECK_MEMSET (256);
  CHECK_MEMSET (257);

  return 0;
}
