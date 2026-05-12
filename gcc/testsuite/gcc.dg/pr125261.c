/* { dg-do compile } */
/* { dg-options "-Wuseless-cast" } */


static int i = _Generic(0., int: (int)0, default: 0);
static int j = _Generic(0, int: 0, default: (int)0, float: 0);
static int k = _Generic(0, default: 0, int: (int)0);	/* { dg-warning "useless" } */

/* If the active assocation cames later than the default, we do
   not know that it is unused.  */
// static int l = _Generic(0, default: (int)0, int: 0);

