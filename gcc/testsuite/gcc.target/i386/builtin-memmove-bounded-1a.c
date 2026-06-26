/* { dg-do run } */
/* { dg-options "-O2 -minline-all-stringops" } */

/* Test -O2 -minline-all-stringops on memmove with various bounds.  */

#include <stdlib.h>

#define MAX_LENGTH 512
#define TOTAL (4 * MAX_LENGTH)

#define MEMMOVE_BOUND(BOUND) \
__attribute__ ((noipa, noinline)) \
static char * \
memmove_##BOUND (char *dest, char *src, size_t len) \
{ \
  if (len <= BOUND) \
    return __builtin_memmove (dest, src, len); \
  else \
    return dest; \
}

#define CHECK_MEMMOVE_BOUND(BOUND, SIZE) \
  { \
    char *dest, *src; \
    unsigned int offset; \
    for (i = 0; i < SIZE; i++) \
      buf[i] = 'a'; \
    for (; i < 2 * SIZE; i++) \
      buf[i] = 'A'; \
    for (; i < 3 * SIZE; i++) \
      buf[i] = 'b'; \
    for (i = 0; i < 3 * SIZE; i++) \
      copy[i] = buf[i]; \
    offset = SIZE - (SIZE) / 2; \
    dest = &buf[0]; \
    src = &buf[offset]; \
    p = memmove_##BOUND (dest, src, SIZE); \
    if (p != dest) \
      abort (); \
    for (i = 0; i < SIZE; i++) \
      if (dest[i] != copy[offset + i]) \
        abort (); \
    for (i = 0; i < 3 * SIZE; i++) \
      buf[i] = copy[i]; \
    dest = &buf[offset]; \
    src = &buf[0]; \
    p = memmove_##BOUND (dest, src, SIZE); \
    if (p != dest) \
      abort (); \
    for (i = 0; i < SIZE; i++) \
      if (dest[i] != copy[i]) \
        abort (); \
  }

#define CHECK_MEMMOVE(SIZE) \
  CHECK_MEMMOVE_BOUND (SIZE, SIZE) \
  if (SIZE > 1) \
    CHECK_MEMMOVE_BOUND (SIZE, SIZE - 1) \
  if (SIZE > 2) \
    CHECK_MEMMOVE_BOUND (SIZE, SIZE - 2) \
  if (SIZE > 3) \
    CHECK_MEMMOVE_BOUND (SIZE, SIZE - 3) \
  if (SIZE > 4) \
    CHECK_MEMMOVE_BOUND (SIZE, SIZE - 4) \
  if (SIZE > 5) \
    CHECK_MEMMOVE_BOUND (SIZE, SIZE - 5) \
  if (SIZE > 6) \
    CHECK_MEMMOVE_BOUND (SIZE, SIZE - 6) \
  if (SIZE > 7) \
    CHECK_MEMMOVE_BOUND (SIZE, SIZE - 7)

char buf[TOTAL];
char copy[TOTAL];

MEMMOVE_BOUND (0);
MEMMOVE_BOUND (1);
MEMMOVE_BOUND (2);
MEMMOVE_BOUND (3);
MEMMOVE_BOUND (4);
MEMMOVE_BOUND (5);
MEMMOVE_BOUND (7);
MEMMOVE_BOUND (8);
MEMMOVE_BOUND (9);
MEMMOVE_BOUND (15);
MEMMOVE_BOUND (16);
MEMMOVE_BOUND (17);
MEMMOVE_BOUND (31);
MEMMOVE_BOUND (32);
MEMMOVE_BOUND (33);
MEMMOVE_BOUND (63);
MEMMOVE_BOUND (64);
MEMMOVE_BOUND (65);
MEMMOVE_BOUND (127);
MEMMOVE_BOUND (128);
MEMMOVE_BOUND (129);
MEMMOVE_BOUND (255);
MEMMOVE_BOUND (256);
MEMMOVE_BOUND (257);

int
main (void)
{
  unsigned int i;
  char *p;

  CHECK_MEMMOVE (0);
  CHECK_MEMMOVE (1);
  CHECK_MEMMOVE (2);
  CHECK_MEMMOVE (3);
  CHECK_MEMMOVE (4);
  CHECK_MEMMOVE (5);
  CHECK_MEMMOVE (7);
  CHECK_MEMMOVE (8);
  CHECK_MEMMOVE (9);
  CHECK_MEMMOVE (15);
  CHECK_MEMMOVE (16);
  CHECK_MEMMOVE (17);
  CHECK_MEMMOVE (31);
  CHECK_MEMMOVE (32);
  CHECK_MEMMOVE (33);
  CHECK_MEMMOVE (63);
  CHECK_MEMMOVE (64);
  CHECK_MEMMOVE (65);
  CHECK_MEMMOVE (127);
  CHECK_MEMMOVE (128);
  CHECK_MEMMOVE (129);
  CHECK_MEMMOVE (255);
  CHECK_MEMMOVE (256);
  CHECK_MEMMOVE (257);

  return 0;
}
