/* { dg-do run } */
/* { dg-options "-fdump-tree-crc-details" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-g" "-Os" "-flto" } } */
/* { dg-require-effective-target int32plus } */

#include <stdlib.h>

typedef unsigned short ee_u16;
typedef unsigned char ee_u8;

__attribute__ ((noinline,optimize(0)))
ee_u16 crcu16_O0 (ee_u16 data, ee_u16 crc) {
  ee_u8 i = 0, x16 = 0, carry = 0;
  for (i = 0; i < 16; i++) {
      x16 = (ee_u8) ((data & 1) ^ ((ee_u8) crc & 1));
      data >>= 1;
      if (x16 == 1) {
	  crc ^= 0x4002;
	  carry = 1;
	} else
	carry = 0;
      crc >>= 1;
      if (carry)
	crc |= 0x8000;
      else
	crc &= 0x7fff;
    }
  return crc;
}

ee_u16 crcu16 (ee_u16 data, ee_u16 crc) {
  ee_u8 i = 0, x16 = 0, carry = 0;
  for (i = 0; i < 16; i++) {
      x16 = (ee_u8) ((data & 1) ^ ((ee_u8) crc & 1));
      data >>= 1;
      if (x16 == 1) {
	  crc ^= 0x4002;
	  carry = 1;
	} else
	carry = 0;
      crc >>= 1;
      if (carry)
	crc |= 0x8000;
      else
	crc &= 0x7fff;
    }
  return crc;
}

int main ()
{
  ee_u16 crc = 0x0D80;
  for (ee_u16 i = 0; i < 255; i++)
    {
      ee_u16 res1 = crcu16_O0 (i, crc);
      ee_u16 res2 = crcu16 (i, crc);
      if (res1 != res2)
	__builtin_abort ();
      crc = res1;
    }
  __builtin_exit (0);
}

/* { dg-final { scan-tree-dump "function maybe contains CRC calculation." "crc" } } */
/* { dg-final { scan-tree-dump "Loop iteration number is 15" "crc"} } */
/* { dg-final { scan-tree-dump "Bit reversed" "crc"} } */
/* { dg-final { scan-tree-dump "calculates CRC!" "crc"} } */
/* { dg-final { scan-tree-dump-times "Couldn't generate faster CRC code." 0 "crc"} } */
