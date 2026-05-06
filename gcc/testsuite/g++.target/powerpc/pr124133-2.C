// PR target/124133
// { dg-options "-mdejagnu-cpu=power8 -mvsx -O2" }
// { dg-require-effective-target powerpc_vsx }

#include <altivec.h>                                                                                                                                                                  
                                                                                                                                                                                      
struct S { float d; unsigned char e[16]; };                                                                                                                                           
                                                                                                                                                                                      
template <typename T>                                                                                                                                                                 
void                                                                                                                                                                                  
foo (const S *s)                                                                                                                                                                      
{                                                                                                                                                                                     
  vector signed char a = (vector signed char) vec_xl (0, s->e);                                                                                                                       
}                                                                                                                                                                                     
