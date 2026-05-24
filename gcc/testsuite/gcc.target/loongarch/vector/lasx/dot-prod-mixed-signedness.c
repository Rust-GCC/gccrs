/* { dg-options "-O3 -mlasx -fno-unroll-loops -fdump-tree-optimized" } */
/* { dg-final { scan-tree-dump-times "DOT_PROD_EXPR" 1 "optimized" } } */

typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned char uint8_t;
typedef char int8_t;
typedef uint32_t IndexType;

[[gnu::noipa]]
void transform(int32_t *__restrict__ output, const int8_t *weights,
               const int32_t *biases, const uint8_t *input) {
  for (IndexType i = 0; i < 0x10; ++i) {
    const IndexType offset = i * 0x1000;

    int32_t sum = biases[i];
    for (IndexType j = 0; j < 0x1000; ++j) {
      sum += weights[offset + j] * input[j];
    }
    output[i] = sum;
  }
}

[[gnu::noipa]]
void transform_novec(int32_t *__restrict__ output,
                     const int8_t *weights,
                     const int32_t *biases,
                     const uint8_t *input) {
#pragma GCC novector
  for (IndexType i = 0; i < 0x10; ++i) {
    const IndexType offset = i * 0x1000;

    int32_t sum = biases[i];
#pragma GCC novector
    for (IndexType j = 0; j < 0x1000; ++j) {
      sum += weights[offset + j] * input[j];
    }
    output[i] = sum;
  }
}

int8_t w[0x11000];
uint8_t in[0x1000];
int32_t out[0x10];
int32_t out_novec[0x10];
int32_t b[0x10];

int
main ()
{
  for (unsigned i = 0; i < 0x11000; i++)
    w[i] = (int8_t)(uint8_t)(i * i + 113 * i + 47);
  for (unsigned i = 0x11000; i < 0x12000; i++)
    in[i - 0x11000] = (uint8_t)(i * i + 113 * i + 47);
  for (int i = 0; i < 0x10; i++)
    b[i] = i;

  transform (out, w, b, in);
  transform_novec (out_novec, w, b, in);

  if (__builtin_memcmp (out, out_novec, sizeof (out)))
    __builtin_trap ();
}
