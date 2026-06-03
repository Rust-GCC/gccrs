! { dg-do compile }
! { dg-additional-options "-O2 -ffp-contract=off" }
! { dg-additional-options "-march=armv8.3-a" { target { aarch64*-*-* } } }

subroutine foo(a,b,c)

  complex :: a(6,6)
  complex :: b(6,6)
  complex :: c(6,6)

  c = MATMUL(a, b)

end subroutine foo

! { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" { target { vect_float } }  } }
! { dg-final { scan-tree-dump-not "Found COMPLEX_MUL" "vect" } }
