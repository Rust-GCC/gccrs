/* Test cond_vec_cbranch_all<mode> for the 256-bit V_ANY_CBRANCH modes
   (V4DI, V4DF).  */
/* { dg-do compile } */
/* { dg-options "-Ofast -mavx512f -mavx512vl -mavx512dq -mprefer-vector-width=256 -fno-schedule-insns -fno-reorder-blocks -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define N 100000
long long a_long[N],   b_long[N],   c_long[N];
double    a_double[N], b_double[N], c_double[N];

/*
** f_v4di:
**	...
**	vpcmpq	\$4, %ymm[0-9]+, %ymm[0-9]+, %k[0-9]+
**	vpcmpq	\$4, %ymm[0-9]+, %ymm[0-9]+, %k[0-9]+\{%k[0-9]+\}
**	kortestb	%k[0-9]+, %k[0-9]+
**	je	\.L[0-9]+
**	...
*/
void f_v4di (long long d, long long e)
{
  for (int i = 0; i < N; i++)
    {
      b_long[i] += a_long[i];
      if (c_long[i] == e) continue;
      if (a_long[i] != d) break;
    }
}

/*
** f_v4df:
**	...
**	vcmppd	\$4, %ymm[0-9]+, %ymm[0-9]+, %k[0-9]+
**	vcmppd	\$4, %ymm[0-9]+, %ymm[0-9]+, %k[0-9]+\{%k[0-9]+\}
**	kortestb	%k[0-9]+, %k[0-9]+
**	je	\.L[0-9]+
**	...
*/
void f_v4df (double d, double e)
{
  for (int i = 0; i < N; i++)
    {
      b_double[i] += a_double[i];
      if (c_double[i] == e) continue;
      if (a_double[i] != d) break;
    }
}
