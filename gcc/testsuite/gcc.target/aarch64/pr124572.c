/* { dg-do compile } */
/* { dg-options "-O2" } */

int e() {
  char f;
  long tt = (long)&f;
  tt+=1;
  int g = tt;
  return g + 3;
}
