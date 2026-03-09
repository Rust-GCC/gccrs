/* Test the correct application of masking to autovectorized math function calls.  */
/* { dg-do compile { target { aarch64*-*-* } } } */
/* { dg-additional-options "-march=armv8.2-a+sve -fdump-tree-ifcvt-raw -Ofast" { target { aarch64*-*-* } } } */

__attribute__ ((__simd__ ("notinbranch"), const)) extern float
expf (float __x) __attribute__ ((__nothrow__ , __leaf__));

const int N = 20;
const float lim = 101.0;
const float cst =  -1.0;
float tot =   0.0;

float b[20];
float a[20] = { [0 ... 9] = 1.7014118e39, /* If branch. */
		[10 ... 19] = 100.0 };    /* Else branch.  */

int main (void)
{
  #pragma omp simd
  for (int i = 0; i < N; i += 1)
    {
      if (a[i] > lim)
	b[i] = cst;
      else
	b[i] = expf (a[i]);
      tot += b[i];
    }
  return (0);
}

/* { dg-final { scan-tree-dump { gimple_call <.MASK_CALL, _[0-9]+, expf, _[0-9]+, _[0-9]+>} ifcvt { target { aarch64*-*-* } } } } */
