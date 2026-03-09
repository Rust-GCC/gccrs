/* { dg-do compile } */
/* { dg-require-effective-target lp64 } */
/* { dg-additional-options "-w" } */
#include <stdint.h>
#define BS_VEC(type, num) type __attribute__((vector_size(num * sizeof(type))))
uint8_t backsmith_snippet_141(int16_t, uint64_t);
int32_t backsmith_snippet_122(uint64_t BS_ARG_0, uint32_t BS_ARG_1)
{
    BS_ARG_0 =
        BS_ARG_1 ? (BS_VEC(uint64_t, 16)){}[BS_ARG_0] : 4054722019416799465;
    return BS_ARG_0;
}
uint16_t backsmith_pure_0(uint64_t BS_ARG_2, uint32_t BS_ARG_3)
{
    int64_t BS_VAR_0[6];
    int8_t BS_VAR_3[80];
    for (uint16_t BS_INC_0 = 0; BS_INC_0 < 8; BS_INC_0 += 1)
    {
        uint64_t BS_TEMP_590 = BS_INC_0;
        BS_VAR_0[BS_INC_0] = BS_INC_0
            ? (BS_TEMP_590 ? BS_ARG_2 >> BS_TEMP_590 : 0)
                ?: backsmith_snippet_141(0, BS_INC_0)
            : 0;
        BS_VAR_3[BS_INC_0] =
            backsmith_snippet_122(BS_VAR_0[6 ? (uint64_t)BS_INC_0 : 0] < 0,
                                  BS_ARG_3)
            ?: BS_ARG_3;
    }
    if (BS_ARG_2) BS_VAR_3[BS_ARG_3 < 80 ? BS_ARG_3 : 0] = 0;
    return BS_VAR_3[4];
}
