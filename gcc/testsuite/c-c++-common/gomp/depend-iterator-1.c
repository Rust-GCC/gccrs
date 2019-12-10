/* { dg-additional-options "-Wno-volatile" { target c++ } } */

int arr[64], arr2[64];
struct S { int a[4]; } k;
short arr4[4];
volatile int v;
#define TEST_EQ(x,y) ({ int o[x == y ? 1 : -1]; 0; })

void
foo (unsigned char i, signed char j)
{
  #pragma omp task depend (iterator (j=6:2:-2) , out : \
	arr[TEST_EQ (sizeof (j), sizeof (int)), \
	    TEST_EQ (sizeof (i), sizeof (unsigned char)), \
	    TEST_EQ (sizeof (k), sizeof (struct S)), j], \
	arr2[TEST_EQ (((__typeof (j)) -1) < 0, 1), \
	     TEST_EQ (((__typeof (i)) -1) < 0, 0), \
	     TEST_EQ (((__typeof (k.a[0])) -1) < 0, 1), j]) \
	depend(out: arr[0]) \
	depend (iterator (long long i=__LONG_LONG_MAX__ - 4:__LONG_LONG_MAX__ - 2:2, \
			  unsigned short j=~0U-16:~0U-8:3, \
			  short *k=&arr4[1]:&arr4[2]:1) , in : \
	arr[TEST_EQ (sizeof (i), sizeof (long long)), \
	    TEST_EQ (sizeof (j), sizeof (unsigned short)), \
	    TEST_EQ (sizeof (k), sizeof (short *)), \
	    TEST_EQ (sizeof (*k), sizeof (short)), i - __LONG_LONG_MAX__ + 4], \
	arr2[TEST_EQ (((__typeof (i)) -1) < 0, 1), \
	     TEST_EQ (((__typeof (j)) -1) < 0, 0), \
	     TEST_EQ (((__typeof (*k)) -1) < 0, 1), j - (~0U-16)], \
	arr2[k - &arr4[0]]) \
	depend(in : k)
    v++;
}

void
bar (unsigned char i, signed char j)
{
  int m = j;
  int n = j + 2;
  #pragma omp task depend (iterator (j=6:2:m) , out : \
	arr[TEST_EQ (sizeof (j), sizeof (int)), \
	    TEST_EQ (sizeof (i), sizeof (unsigned char)), \
	    TEST_EQ (sizeof (k), sizeof (struct S)), j], \
	arr2[TEST_EQ (((__typeof (j)) -1) < 0, 1), \
	     TEST_EQ (((__typeof (i)) -1) < 0, 0), \
	     TEST_EQ (((__typeof (k.a[0])) -1) < 0, 1), j]) \
	depend(out: arr[0]) \
	depend (iterator (long long i=__LONG_LONG_MAX__ - 4 - n:__LONG_LONG_MAX__ - 2:2, \
			  unsigned short j=~0U-16:~0U-8-n:3, \
			  short *k=&arr4[1]:&arr4[n + 2]:1) , in : \
	arr[TEST_EQ (sizeof (i), sizeof (long long)), \
	    TEST_EQ (sizeof (j), sizeof (unsigned short)), \
	    TEST_EQ (sizeof (k), sizeof (short *)), \
	    TEST_EQ (sizeof (*k), sizeof (short)), i - __LONG_LONG_MAX__ + 4], \
	arr2[TEST_EQ (((__typeof (i)) -1) < 0, 1), \
	     TEST_EQ (((__typeof (j)) -1) < 0, 0), \
	     TEST_EQ (((__typeof (*k)) -1) < 0, 1), j - (~0U-16)], \
	arr2[k - &arr4[0]:10]) \
	depend(in : k)
    v++;
}

void
baz (void)
{
  #pragma omp parallel
  #pragma omp master
  {
    #pragma omp task depend(iterator(unsigned long int k = 0 : 2) , inout : \
	arr[TEST_EQ (sizeof (k), sizeof (unsigned long)), \
	    TEST_EQ (((__typeof (k)) -1) < 0, 0), k]) \
	depend(iterator(signed char s = -3 : -12 : -1) , out : \
	arr[TEST_EQ (sizeof (s), sizeof (signed char)), \
	    TEST_EQ (((__typeof (s)) -1) < 0, 1), s + 12])
      v++;
  }
}
