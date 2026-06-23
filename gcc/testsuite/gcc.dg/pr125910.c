/* { dg-do compile } */
/* { dg-options "-O2 -fno-delete-null-pointer-checks" } */

void a();
extern int b;
int *c;
void d() {
  int *e = c;
  if (e)
    a();
  if (e == &b)
    a();
  if (e)
    a();
}
