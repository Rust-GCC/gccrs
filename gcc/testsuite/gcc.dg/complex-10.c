/* { dg-do compile } */
/* { dg-options "" } */

/* PR c/105555 */

struct C {
  __complex int i;
};
struct C p[10];
int *foo1(void) {
  return &__real(p->i);
}

int *foo2(void) {
  return &__imag(p->i);
}
