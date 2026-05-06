/* { dg-do compile } */
/* { dg-options "-O2 -Wno-psabi -fdump-tree-forwprop1" } */

#define vector4 __attribute__((vector_size(4*sizeof(int))))
#define vector2_u64 __attribute__((vector_size(2*sizeof(unsigned long))))

vector4 int g1 (vector4 int x) {
  vector4 int cst1 = {5, 1};
  vector4 int cst2 = {10, 2};
  vector4 int cst3 = {15, 3};
  return x != cst1 ? x + cst2 : cst3;
}

vector2_u64 unsigned long g2 (vector2_u64 unsigned long x) { 
  return x != (vector2_u64 unsigned long){10, 10}
	 ? x + (vector2_u64 unsigned long){20, 20}
	 : (vector2_u64 unsigned long){40, 40}; 
}

/* { dg-final { scan-tree-dump-times "VEC_COND_EXPR" 2 "forwprop1" } } */
