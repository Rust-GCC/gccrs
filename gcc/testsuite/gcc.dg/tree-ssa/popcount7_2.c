/* { dg-do compile } */
/* { dg-require-effective-target popcount } */
/* { dg-require-effective-target int32plus } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

const unsigned m1  = 0x55555555UL;
const unsigned m2  = 0x33333333UL;
const unsigned m3  = 0x0F0F0F0FUL;
const unsigned m4  = 0x00FF00FFUL;
const unsigned m5  = 0x0000FFFFUL;

int Gia_WordCountOnes32c( unsigned uword )
{
  uword = (uword & m1) + ((uword>>1) & m1);
  uword = (uword & m2) + ((uword>>2) & m2);
  uword = (uword & m3) + ((uword>>4) & m3);
  uword = (uword & m4) + ((uword>>8) & m4);
  return  (uword & m5) + ((uword>>16) & m5);
}

/* { dg-final { scan-tree-dump-times "\.POPCOUNT" 1 "optimized" } } */
