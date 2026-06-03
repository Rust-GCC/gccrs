// { dg-do run }
// { dg-options "-O3" }

#include <stdint.h>
int64_t g2, g27, g10;
static void *g13, *g24;
_Bool f11___trans_tmp_1, f11_c17;
void __attribute__((noinline)) f11(int64_t a0, void *a4, void *a5)
{
    _Bool ob6;
    int32_t v12;
    int64_t v15;
lbl_entry:
lbl_sw_def4:
    ob6 = g13 == a4;
    if (ob6 || f11_c17) return;
    if (f11___trans_tmp_1) v12 = g10;
    a0 = (char *)a5 - (char *)a4;
    ob6 = 0 == a0;
    *(_Bool *)4 = 0;
    if (ob6) goto lbl_b22;
    __builtin_unreachable();
lbl_b22:
    g27 = v12;
    switch (v15)
    {
        case 6009053803839173278: goto lbl_sw_def4;
        case 70689892217558167: goto lbl_entry;
    }
}

int main()
{
    f11(0, 0, g24);
}
