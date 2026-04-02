/* { dg-do compile } */
/* { dg-options "-O2 -mcpu=v10.0 -mxl-barrel-shift -S" } */
 
unsigned short t = 0x1234;

extern unsigned short int g();

int f()
{
  unsigned short s = t;

/* { dg-final { scan-assembler-not "brlid\tr(\[0-9]\|\[1-2]\[0-9]\|3\[0-1]),g\n\[ \t\n\]*swap" } } */
  if (__builtin_bswap16(s) != g()) {
	  return -1;
  }

  return 0;
}


