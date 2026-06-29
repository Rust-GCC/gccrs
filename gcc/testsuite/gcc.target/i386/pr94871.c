/* PR target/94871 */
/* { dg-do compile } */
/* { dg-options "-O2 -mavx2 -march=skylake" } */

typedef double v2df __attribute__((vector_size(16)));
typedef long long v2di __attribute__((vector_size(16)));
typedef char v16qi __attribute__((vector_size(16)));

v2df p, q;
v2di out;

void foo()
{
  signed char a = 0xff;
  v2di tmp = (v2di)(v16qi){a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a};
  out = ((v2di)__builtin_ia32_cmpeqpd(p, q) ^ tmp);
}

void bar()
{
  signed char a = 0xff;
  v2di tmp = (v2di)(v16qi){a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a};
  out = ((v2di)__builtin_ia32_cmpneqpd(p, q) ^ tmp);
}

/* { dg-final { scan-assembler-not "xor" } } */
