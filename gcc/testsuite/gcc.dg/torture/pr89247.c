/* { dg-do compile } */
/* { dg-require-effective-target label_values } */

int *a;
void b()
{
  void *c = &&d;
  for (;;)
    d:
	if (*a)
	  ;
	else
	  *a = ({ 0 < b; });
}
