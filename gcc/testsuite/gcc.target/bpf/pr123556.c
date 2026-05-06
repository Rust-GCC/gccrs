/* PR target/123556.
   Test that we do not load the bit-fields with a larger aligned load
   that crosses struct boundaries, which the BPF verifier rejects.  */
/* { dg-do compile } */
/* { dg-options "-O2 -S -dA -masm=normal" } */

struct problem
{
  unsigned char f1;
  unsigned char f2;
  struct {
    unsigned short id;
    unsigned short val;
  } stmemb[2];

  unsigned char ba:2;
  unsigned char bb:3;

  unsigned short f3;
};

int foo (struct problem *p)
{
  if (p->bb != 1)
    return -1;
  if (p->ba >= 2)
    return -1;

  return 0;
}

/* { dg-final { scan-assembler-not "ldxw" } } */
/* { dg-final { scan-assembler-not "ldxdw" } } */
