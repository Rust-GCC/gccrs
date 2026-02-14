! { dg-do compile }
! { dg-options "-fcoarray=lib -fdump-tree-original" }
!
! Test the coshape intrinsic (PR99250)
! 
program coshape_1
   integer, Parameter :: i4 = kind (1_4), i8 = kind (1_8)
   real, codimension[-1:*] :: cr
   real, dimension(4,4), codimension[0:2,*] :: cr2
   integer(i4) :: no_images, val4(2)
   integer(i8), allocatable :: val8(:)

   no_images = num_images()

   if (this_image() == 1) then

! First without the KIND argument...
      val4(1:1) = coshape(cr)
      if (val4(1) /= no_images) stop 1
      if (val4(1) /= 1 + ucobound (cr, 1, i4) - lcobound (cr, 1, i4)) stop 2
      if (mod (no_images,3) == 0) then
         val4 = coshape(cr2)
         if (val4(1) /= 3 .or. product (val4(1:2)) /= no_images) stop 3
         if (val4(2) /= 1 + ucobound (cr2, 2, i4) - lcobound (cr2, 2, i4)) stop 4
      else
         print *, "No. images must be a multiple of 3 for the coshape test #"
      endif

! ...then with it
      if (kind (coshape(cr, kind = i4)) /= i4) stop 5
      if (kind (coshape(cr, kind = i8)) /= i8) stop 6

      val8 = coshape(cr, kind = i8)
      if (val8(1) /= 1 + ucobound (cr, 1, i8) - lcobound (cr, 1, i8)) stop 7
      if (val8(1) /= no_images) stop 8
      if (mod (no_images,3) == 0) then
         val8 = coshape(cr2, kind = i8)
         if (val8(1) /= 3 .or. product (val8(1:2)) /= no_images) stop 9
         if (val8(2) /= 1 + ucobound (cr2, 2, i8) - lcobound (cr2, 2, i8)) stop 10
      else
         print *, "No. images must be a multiple of 3 for the coshape test #"
      endif
      if (any (shape(cr2) /= [4,4])) stop 11
   endif

end program coshape_1
! { dg-final { scan-tree-dump-times "_gfortran_caf_num_images" 9 "original" } }
