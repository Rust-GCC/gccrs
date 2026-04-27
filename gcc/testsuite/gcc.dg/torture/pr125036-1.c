/* PR tree-optimization/125036 */
/* { dg-do compile } */

enum a { b, c, d, e, f };
int g;
void h(int *);
void j() {
  enum a i = b;
  for (; i < f; ++i)
    h(&g);
}
