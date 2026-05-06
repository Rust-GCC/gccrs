/* { dg-do run } */
/* { dg-shouldfail "" } */

/* Raise red flags if the way programs are meant to flag failures aren't
   detected as such.  This is unlikely to indicate a compiler problem, but
   maybe a runtime or test infrastructure problem.  */

int main() {
  __builtin_exit(-1);
}
