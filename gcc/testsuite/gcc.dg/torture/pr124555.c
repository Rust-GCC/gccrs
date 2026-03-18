/* { dg-additional-options "-fwrapv" } */

int vsad8_c_y;
char *vsad8_c_s1, *vsad8_c_s2;
long vsad8_c_stride;
int vsad8_c()
{
  int score, x;
  for (; vsad8_c_y; vsad8_c_y++)
    {
      x = 0;
      for (; x < 8; x++)
	score += (vsad8_c_s1[x] - vsad8_c_s2[x] - vsad8_c_s1[x + vsad8_c_stride]
	    + vsad8_c_s2[x + vsad8_c_stride]) >= 0
	    ? vsad8_c_s1[x] - vsad8_c_s2[x] -
	    vsad8_c_s1[x + vsad8_c_stride] +
	    vsad8_c_s2[x + vsad8_c_stride]
	    : -(vsad8_c_s1[x] - vsad8_c_s2[x] -
		vsad8_c_s1[x + vsad8_c_stride] +
		vsad8_c_s2[x + vsad8_c_stride]);
      vsad8_c_s1 += vsad8_c_stride;
      vsad8_c_s2 += vsad8_c_stride;
    }
  return score;
}
