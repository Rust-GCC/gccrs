! { dg-do run }
program foo
   integer(1) :: i1 = 4
   integer(2) :: i2 = 4
   integer(4) :: i4 = 4, dm, nc
   integer(8) :: i8 = 4
   real(4) :: r4 = 4
   real(8) :: r8 = 4
   complex(4) :: c4 = 4
   complex(8) :: c8 = 4
   if (any(shape(spread(i1,1,-1)) /= 0)) stop 1
   if (any(shape(spread(i2,1,-1)) /= 0)) stop 2
   if (any(shape(spread(i4,1,-1)) /= 0)) stop 3
   if (any(shape(spread(i8,1,-1)) /= 0)) stop 4
   if (any(shape(spread(r4,1,-1)) /= 0)) stop 5
   if (any(shape(spread(r8,1,-1)) /= 0)) stop 6
   if (any(shape(spread(c4,1,-1)) /= 0)) stop 7
   if (any(shape(spread(c8,1,-1)) /= 0)) stop 8
   dm = 1
   nc = -1
   if (any(shape(spread(i1,dm,nc)) /= 0)) stop 9
   if (any(shape(spread(i2,dm,nc)) /= 0)) stop 10
   if (any(shape(spread(i4,dm,nc)) /= 0)) stop 11
   if (any(shape(spread(i8,dm,nc)) /= 0)) stop 12
   if (any(shape(spread(r4,dm,nc)) /= 0)) stop 13
   if (any(shape(spread(r8,dm,nc)) /= 0)) stop 14
   if (any(shape(spread(c4,dm,nc)) /= 0)) stop 15
   if (any(shape(spread(c8,dm,nc)) /= 0)) stop 16
end program foo
