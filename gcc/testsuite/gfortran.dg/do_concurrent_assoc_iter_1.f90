! { dg-do run }
! PR fortran/125532
! DO CONCURRENT with inline type-spec iterator used inside an ASSOCIATE body
! produced wrong values: replace_in_code_recursive lacked an EXEC_BLOCK case
! so shadow iterator references were not replaced.
program do_concurrent_assoc_iter_1
  implicit none
  integer :: a(5), b(5)
  a = 0
  b = 0
  ! Iterator k used in the ASSOCIATE selector and in the body.
  do concurrent (integer :: k = 1:5)
    associate (sq => k * k)
      a(k) = sq
    end associate
  end do
  ! Iterator k used only inside the body (not in selector).
  do concurrent (integer :: k = 1:5)
    associate (v => k)
      b(v) = k + 10
    end associate
  end do
  if (any (a /= [1, 4, 9, 16, 25])) stop 1
  if (any (b /= [11, 12, 13, 14, 15])) stop 2
end program
