/* { dg-do compile } */
/* { dg-options "-O2 -Wno-psabi -fdump-tree-forwprop1" } */

#define vector4 __attribute__((vector_size(4*sizeof(int))))
#define vector4_u8 __attribute__((vector_size(4*sizeof(unsigned char))))
#define vector4_u32 __attribute__((vector_size(4*sizeof(unsigned int))))
#define vector2_u64 __attribute__((vector_size(2*sizeof(unsigned long))))

void unopt(vector4 int *v) {
  vector4 int t = *v;
  vector4 int t1 = t + 8;
  *v = (t != -8) ? (t1) : (vector4 int){0, 0, 0, 0};
}

vector2_u64 unsigned long f1 (vector2_u64 unsigned long x) { 
  return x != (vector2_u64 unsigned long){5, 5}
	 ? x + (vector2_u64 unsigned long){10, 10}
	 : (vector2_u64 unsigned long){15, 15}; 
}

vector4_u32 unsigned int f2 (vector4_u32 unsigned int x)
{ 
  return x != (vector4_u32 unsigned int){20, 20, 20, 20}
	 ? x - (vector4_u32 unsigned int){5, 5, 5, 5}
	 : (vector4_u32 unsigned int){15, 15, 15, 15}; 
}

vector4_u8 unsigned char
f3 (vector4_u8 unsigned char x)
{
  return x != (vector4_u8 unsigned char){100, 100, 100, 100}
	 ? x + (vector4_u8 unsigned char){166, 166, 166, 166}
	 : (vector4_u8 unsigned char){10, 10, 10, 10};
}

/* { dg-final { scan-tree-dump-not "VEC_COND_EXPR" "forwprop1" } } */
