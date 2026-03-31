/* { dg-do link  { target { rv64 } } } */
/* { dg-options "-march=rv64gc_zbb_zbs -mabi=lp64d -flto" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" "-O1" } } */

struct S {
  int a;
  char b;
  int c;
} s;

const signed char c = -37;
int d;
struct S v1[] = {{0, 8}, 0, 0, -108976}, v2[] = {{}, 0, 0, 2804};
int a;
struct S v3[3];
int *p = &a;

void foo() {
  int a;
  if (a)
    ;
  else if (v1[0].b)
    s.a = 0;
  else
    d = 0;
  if (*p)
    if (v3[1].c)
      if (1 ^ (d & c & v2[1].c & ~v1[1].c | s.a))
        v3[2].c = 0;
}

int main() { foo(); }
