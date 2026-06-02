/* Test cond_vec_cbranch_all<mode> for the 128-bit V_ANY_CBRANCH modes
   (V4SI, V2DI, V4SF, V2DF).  */
/* { dg-do compile } */
/* { dg-options "-Ofast -mavx512f -mavx512vl -mavx512dq -mprefer-vector-width=128 -fno-schedule-insns -fno-reorder-blocks -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define N 100000
int       a_int[N],    b_int[N],    c_int[N];
long long a_long[N],   b_long[N],   c_long[N];
float     a_float[N],  b_float[N],  c_float[N];
double    a_double[N], b_double[N], c_double[N];

/*
** f_v4si:
**	...
**	vpcmpd	\$4, %xmm[0-9]+, %xmm[0-9]+, %k[0-9]+
**	vpcmpd	\$4, %xmm[0-9]+, %xmm[0-9]+, %k[0-9]+\{%k[0-9]+\}
**	kortestb	%k[0-9]+, %k[0-9]+
**	je	\.L[0-9]+
**	...
*/
void f_v4si (int d, int e)
{
  for (int i = 0; i < N; i++)
    {
      b_int[i] += a_int[i];
      if (c_int[i] == e) continue;
      if (a_int[i] != d) break;
    }
}

/*
** f_v2di:
**	...
**	vpcmpq	\$4, %xmm[0-9]+, %xmm[0-9]+, %k[0-9]+
**	vpcmpq	\$4, %xmm[0-9]+, %xmm[0-9]+, %k[0-9]+\{%k[0-9]+\}
**	kortestb	%k[0-9]+, %k[0-9]+
**	je	\.L[0-9]+
**	...
*/
void f_v2di (long long d, long long e)
{
  for (int i = 0; i < N; i++)
    {
      b_long[i] += a_long[i];
      if (c_long[i] == e) continue;
      if (a_long[i] != d) break;
    }
}

/*
** f_v4sf:
**	...
**	vcmpps	\$4, %xmm[0-9]+, %xmm[0-9]+, %k[0-9]+
**	vcmpps	\$4, %xmm[0-9]+, %xmm[0-9]+, %k[0-9]+\{%k[0-9]+\}
**	kortestb	%k[0-9]+, %k[0-9]+
**	je	\.L[0-9]+
**	...
*/
void f_v4sf (float d, float e)
{
  for (int i = 0; i < N; i++)
    {
      b_float[i] += a_float[i];
      if (c_float[i] == e) continue;
      if (a_float[i] != d) break;
    }
}

/*
** f_v2df:
**	...
**	vcmppd	\$4, %xmm[0-9]+, %xmm[0-9]+, %k[0-9]+
**	vcmppd	\$4, %xmm[0-9]+, %xmm[0-9]+, %k[0-9]+\{%k[0-9]+\}
**	kortestb	%k[0-9]+, %k[0-9]+
**	je	\.L[0-9]+
**	...
*/
void f_v2df (double d, double e)
{
  for (int i = 0; i < N; i++)
    {
      b_double[i] += a_double[i];
      if (c_double[i] == e) continue;
      if (a_double[i] != d) break;
    }
}
