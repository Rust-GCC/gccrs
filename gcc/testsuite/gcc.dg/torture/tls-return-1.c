/* { dg-do compile } */
/* { dg-options "" } */


/* PR tree-optimization/124099 */ 

struct s1 { int t[2];};
__thread struct s1 t;
struct s1 g()
{ 
  return t;
}
