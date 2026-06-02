/* { dg-do compile } */

#include <stdint.h>

typedef int64_t v8i64 __attribute__((vector_size(64)));
int64_t g2, g17;
void *g24;
static v8i64 g25;

void hash()
{
  v8i64 vec6;
lbl_b3:
  *(v8i64 *)g24 = g25;
  uint64_t __ov_tmp_g17 = __builtin_sub_overflow(g2, g2, &__ov_tmp_g17);
  g17 = __ov_tmp_g17;
  g25 = vec6;
  vec6 = *(v8i64 *)g24;
  g2 = vec6[0];
  goto lbl_b3;
  g25[7];
}
