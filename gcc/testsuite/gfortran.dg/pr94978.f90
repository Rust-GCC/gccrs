! { dg-do compile }
! { dg-options "-Wdo-subscript" }

implicit none

integer(4) :: i, j
integer(4) :: pascal(0:8, 0:8)

do i = 0, 8
  pascal(i, 0) = 1
  do j = 1, i - 1
    pascal(i, j) = pascal(i - 1, j) + pascal(i - 1, j - 1) ! { dg-bogus "Array reference at \\(1\\) out of bounds" }
  end do
  do j = i, 8
    pascal(i, j) = 0
  end do
  pascal(i, i) = 1
end do

do i = 0, 8
  print '(9I4)', pascal(i, :)
end do

end
