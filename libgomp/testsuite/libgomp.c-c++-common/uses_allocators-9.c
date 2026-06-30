#include <stdint.h>
#include <omp.h>

int main (void)
{
  omp_allocator_handle_t my_alloc;
  const omp_alloctrait_t trait[] = { { omp_atk_alignment, 128 } };

  #pragma omp target uses_allocators (traits (trait): my_alloc)
  {
    int *x = (int *) omp_alloc (sizeof (int), my_alloc);
    if ((uintptr_t) x % 128 != 0)
      __builtin_abort ();
    omp_free (x, my_alloc);
  }

  my_alloc = (omp_allocator_handle_t) 0xABCD;

  #pragma omp target uses_allocators (traits (trait): my_alloc)
  {
    int *x = (int *) omp_alloc (sizeof (int), my_alloc);
    if ((uintptr_t) x % 128 != 0)
      __builtin_abort ();
    omp_free (x, my_alloc);
  }

  if (my_alloc != (omp_allocator_handle_t) 0xABCD)
    __builtin_abort ();

  /* The following creates an allocator with empty traits + default mem space. */
  #pragma omp target uses_allocators (my_alloc)
  {
    int *x = (int *) omp_aligned_alloc (128, sizeof (int), my_alloc);
    if ((uintptr_t) x % 128 != 0)
      __builtin_abort ();
    omp_free (x, my_alloc);
  }

  if (my_alloc != (omp_allocator_handle_t) 0xABCD)
    __builtin_abort ();

  return 0;
}

