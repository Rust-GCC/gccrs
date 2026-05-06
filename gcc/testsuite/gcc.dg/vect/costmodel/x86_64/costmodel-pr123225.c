/* { dg-do compile } */
/* { dg-additional-options "-msse4" } */

short *
foo (short *arr)
{
  unsigned int pos = 0;
  while(1)
    {
      arr++;
      if (*arr == 0)
	break;
    }
  return arr;
}

/* { dg-final { scan-tree-dump-not "optimized" "vect" } } */
