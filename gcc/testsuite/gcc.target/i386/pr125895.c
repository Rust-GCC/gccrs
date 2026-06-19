/* { dg-do compile } */
/* { dg-options "-O2 -march=x86-64" } */

__attribute__((__vector_size__(2 * sizeof(float)))) float b;
void a(float d) {
  double c;
  b -= *(float *)__builtin_memcpy(&d, &c, 1);
  __asm__("");
  a(2);
}
