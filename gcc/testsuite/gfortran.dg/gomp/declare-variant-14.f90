! { dg-do compile { target { { i?86-*-* x86_64-*-* } && vect_simd_clones } } } */
! { dg-additional-options "-mno-sse3 -O1 -fdump-tree-gimple -fdump-tree-optimized" }

module main

implicit none

interface
  integer function f01 (x)
    integer, intent (in) :: x
  end function

  integer function f02 (x)
    integer, intent (in) :: x
  end function

  integer function f03 (x)
    integer, intent (in) :: x
  end function

  integer function f04 (x)
    integer, intent(in) :: x

    !$omp declare variant (f01) match (device={isa("avx512f")}) ! 4 or 8
    !$omp declare variant (f02) match (implementation={vendor(score(3):gnu)},device={kind(cpu)}) ! (1 or 2) + 3
    !$omp declare variant (f03) match (implementation={vendor(score(5):gnu)},device={kind(host)}) ! (1 or 2) + 5
  end function

end interface

contains

  integer function test1 (x)
    !$omp declare simd
    integer, intent (in) :: x
    integer :: a, b

    ! At gimplification time, we can't decide yet which function to call.
    ! { dg-final { scan-tree-dump-times "f04 \\\(x" 2 "gimple" { target { !aarch64*-*-* } } } }
    ! After simd clones are created, the original non-clone test1 shall
    ! call f03 (score 6), the sse2/avx/avx2 clones too, but avx512f clones
    ! shall call f01 with score 8.
    ! { dg-final { scan-tree-dump-not "f04 \\\(x" "optimized" } }
    ! { dg-final { scan-tree-dump-times "f03 \\\(x" 14 "optimized" } }
    ! { dg-final { scan-tree-dump-times "f01 \\\(x" 4 "optimized" } }
    a = f04 (x)
    b = f04 (x)
    test1 = a + b
  end function
end module
