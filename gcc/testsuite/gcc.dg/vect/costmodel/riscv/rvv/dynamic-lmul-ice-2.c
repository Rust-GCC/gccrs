/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32 -Ofast -ftree-vectorize -mrvv-max-lmul=dynamic" } */

typedef struct rtx_def *rtx;
struct replacement {
    rtx *where;
    rtx *subreg_loc;
    int mode;
};
static struct replacement replacements[150];
void move_replacements (rtx *x, rtx *y, int n_replacements)
{
  int i;
  for (i = 0; i < n_replacements; i++)
    if (replacements[i].subreg_loc == x)
      replacements[i].subreg_loc = y;
    else if (replacements[i].where == x) 
      {
	replacements[i].where = y;
	replacements[i].subreg_loc = 0;
      }
}
