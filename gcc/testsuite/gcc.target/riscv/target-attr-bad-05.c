/* { dg-do compile } */
/* { dg-skip-if "" { *-*-* } { "-fno-fat-lto-objects" } } */
/* { dg-options "-march=rv64gc -O2 -mabi=lp64" } */


long foo(long a, long b) __attribute__((target(""))); /* { dg-warning "empty string in attribute .target." } */
long foo(long a, long b){
  return a + (b * 2);
}

long bar(long a, long b){
  return a + (b * 2);
}
