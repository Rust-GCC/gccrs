/* { dg-do compile } */
/* { dg-options "-O -fdump-tree-release_ssa -Wno-psabi" } */

typedef signed int s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef __attribute__((vector_size(4 * sizeof (unsigned int)))) unsigned int v4ui;

s32
sf (s32 a, s32 b)
{
  s32 t1 = (~a) & b;
  s32 t2 = a | b;
  return t1 ^ t2;
}

s32
sg (s32 a, s32 b)
{
  s32 t1 = (~a) | b;
  s32 t2 = a & b;
  return t1 ^ t2;
}

u8
f1 (u8 a, u8 b)
{
  u8 t1 = (~a) & b;
  u8 t2 = a | b;
  return t1 ^ t2;
}

u8
g1 (u8 a, u8 b)
{
  u8 t1 = (~a) | b;
  u8 t2 = a & b;
  return t1 ^ t2;
}

u16
f2 (u16 a, u16 b)
{
  u16 t1 = (~a) & b;
  u16 t2 = a | b;
  return t1 ^ t2;
}

u16
g2 (u16 a, u16 b)
{
  u16 t1 = (~a) | b;
  u16 t2 = a & b;
  return t1 ^ t2;
}

u32
f3 (u32 a, u32 b)
{
  u32 t1 = (~a) & b;
  u32 t2 = a | b;
  return t1 ^ t2;
}

u32
g3 (u32 a, u32 b)
{
  u32 t1 = (~a) | b;
  u32 t2 = a & b;
  return t1 ^ t2;
}

u64
f4 (u64 a, u64 b)
{
  u64 t1 = (~a) & b;
  u64 t2 = a | b;
  return t1 ^ t2;
}

u64
g4 (u64 a, u64 b)
{
  u64 t1 = (~a) | b;
  u64 t2 = a & b;
  return t1 ^ t2;
}

_Bool
bf (_Bool a, _Bool b)
{
  _Bool t1 = (~a) & b;
  _Bool t2 = a | b;
  return t1 ^ t2;
}

_Bool
bg (_Bool a, _Bool b)
{
  _Bool t1 = (~a) | b;
  _Bool t2 = a & b;
  return t1 ^ t2;
}

v4ui
vf (v4ui a, v4ui b)
{
  v4ui t1 = (~a) & b;
  v4ui t2 = a | b;
  return t1 ^ t2;
}

v4ui
vg (v4ui a, v4ui b)
{
  v4ui t1 = (~a) | b;
  v4ui t2 = a & b;
  return t1 ^ t2;
}

/* f* and vf should simplify to return a.  */
/* { dg-final { scan-tree-dump-times "return a_|<retval> = a_" 6 "release_ssa" } } */

/* g* and vg should simplify to ~a directly.  */
/* { dg-final { scan-tree-dump-times "= ~a" 6 "release_ssa" } } */
