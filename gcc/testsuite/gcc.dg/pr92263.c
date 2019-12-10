/* { dg-do compile } */
/* { dg-options "-fno-tree-dce -fno-tree-forwprop -Os -ffloat-store" } */

extern long double cabsl (_Complex long double);

typedef struct {
  int nsant, nvqd;
  _Complex long double *vqd;
} vsorc_t;
vsorc_t vsorc;

void foo(int next_job, int ain_num, int iped, long t) {
  long double zpnorm;

  while (!next_job)
    if (ain_num)
    {
      if (iped == 1)
        zpnorm = 0.0;
      int indx = vsorc.nvqd-1;
      vsorc.vqd[indx] = t*1.0fj;
      if (cabsl(vsorc.vqd[indx]) < 1.e-20)
        vsorc.vqd[indx] = 0.0fj;
      zpnorm = t;
      if (zpnorm > 0.0)
        iped = vsorc.nsant;
    }
}
