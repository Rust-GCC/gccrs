program main
  use omp_lib
  use iso_c_binding
  implicit none
  integer(c_intptr_t) :: iptr
  integer (omp_allocator_handle_kind) :: my_alloc
  type(omp_alloctrait), parameter :: trait(*) = &
       [omp_alloctrait(omp_atk_alignment, 128)]

  !$omp target uses_allocators(traits(trait) : my_alloc)
  block
    type(c_ptr) :: ptr
    integer(c_size_t) :: sz = 32
    ptr = omp_alloc (sz, my_alloc)
    if (mod (TRANSFER (ptr, iptr), 128) /= 0) &
      stop 1
    call omp_free (ptr, my_alloc)
  end block

  my_alloc = transfer(int(z'ABCD', omp_allocator_handle_kind), my_alloc)

  !$omp target uses_allocators(traits(trait) : my_alloc)
  block
    type(c_ptr) :: ptr
    integer(c_size_t) :: sz = 32
    ptr = omp_alloc (sz, my_alloc)
    if (mod (TRANSFER (ptr, iptr), 128) /= 0) &
      stop 2
    call omp_free (ptr, my_alloc)
  end block

  if (transfer(my_alloc, 0_omp_allocator_handle_kind) /= int(z'ABCD', &
       omp_allocator_handle_kind)) &
    stop 3

  ! The following creates an allocator with empty traits + default mem space.
  !$omp target uses_allocators(my_alloc)
  block
    type(c_ptr) :: ptr
    integer(c_size_t) :: sz = 32
    ptr = omp_aligned_alloc (128_c_size_t, sz, my_alloc)
    if (mod (TRANSFER (ptr, iptr), 128) /= 0) &
      stop 4
    call omp_free (ptr, my_alloc)
  end block

  if (transfer(my_alloc, 0_omp_allocator_handle_kind) /= int(z'ABCD', &
       omp_allocator_handle_kind)) &
    stop 5

end program main
