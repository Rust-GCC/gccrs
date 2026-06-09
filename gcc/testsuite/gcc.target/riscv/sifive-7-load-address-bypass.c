/* Test SiFive-7 load-to-address bypass optimization.  */

/* { dg-do compile } */
/* { dg-options "-mtune=sifive-7-series" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Os" "-Oz" "-Og" "-flto" } } */
/* { dg-final { check-function-bodies "**" "" } } */

struct node {
  int value;
  struct node *next;
};

/*
**load_value:
**	l[wd]	[at][0-9],0\(a0\)
**	lw	a0,0\([at][0-9]\)
**	ret
*/
int load_value(struct node **ptr)
{
  struct node *p = *ptr;
  return p->value;
}
