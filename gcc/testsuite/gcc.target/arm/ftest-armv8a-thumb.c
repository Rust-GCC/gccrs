/* { dg-do compile } */
/* { dg-skip-if "avoid conflicting multilib options" { *-*-* } { "-march=*" } { "-march=armv8-a" } } */
/* { dg-skip-if "avoid conflicting multilib options" { *-*-* } { "-marm" } { "" } } */
/* { dg-skip-if "-mpure-code supports M-profile only" { *-*-* } { "-mpure-code" } } */
/* { dg-options "-mthumb" } */
/* { dg-add-options arm_arch_v8a } */

#define NEED_ARM_ARCH
#define VALUE_ARM_ARCH 8

#define NEED_ARM_ARCH_ISA_ARM
#define VALUE_ARM_ARCH_ISA_ARM 1

#define NEED_ARM_ARCH_ISA_THUMB
#define VALUE_ARM_ARCH_ISA_THUMB 2

#define NEED_ARM_ARCH_PROFILE
#define VALUE_ARM_ARCH_PROFILE 'A'

#define NEED_ARM_FEATURE_UNALIGNED
#define VALUE_ARM_FEATURE_UNALIGNED 1

#define NEED_ARM_FEATURE_LDREX
#define VALUE_ARM_FEATURE_LDREX 15

#define NEED_ARM_FEATURE_CLZ
#define VALUE_ARM_FEATURE_CLZ 1

#define NEED_ARM_FEATURE_DSP
#define VALUE_ARM_FEATURE_DSP 1

#define NEED_ARM_FEATURE_SIMD32
#define VALUE_ARM_FEATURE_SIMD32 1

#define NEED_ARM_FEATURE_QBIT
#define VALUE_ARM_FEATURE_QBIT 1

#define NEED_ARM_FEATURE_SAT
#define VALUE_ARM_FEATURE_SAT 1

#include "ftest-support.h"
