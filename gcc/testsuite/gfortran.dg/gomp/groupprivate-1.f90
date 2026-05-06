module m
 implicit none
 integer :: ii
 integer :: x, y(20), z, v, u, k
! { dg-error "Sorry, OMP GROUPPRIVATE not implemented, used by 'x' declared at .1." "" { target *-*-* } .-1 }
! { dg-error "Sorry, OMP GROUPPRIVATE not implemented, used by 'y' declared at .1." "" { target *-*-* } .-2 }
! { dg-error "Sorry, OMP GROUPPRIVATE not implemented, used by 'z' declared at .1." "" { target *-*-* } .-3 }
! { dg-error "Sorry, OMP GROUPPRIVATE not implemented, used by 'v' declared at .1." "" { target *-*-* } .-4 }
! { dg-error "Sorry, OMP GROUPPRIVATE not implemented, used by 'u' declared at .1." "" { target *-*-* } .-5 }
! { dg-error "Sorry, OMP GROUPPRIVATE not implemented, used by 'k' declared at .1." "" { target *-*-* } .-6 }
!
 !$omp groupprivate(x, z) device_Type( any )
 !$omp declare target local(x) device_type ( any )
 !$omp declare target enter( ii) ,local(y), device_type ( host )
 !$omp groupprivate(y) device_type( host)
 !$omp groupprivate(v) device_type (nohost )
 !$omp groupprivate(u)

 !$omp groupprivate(k)
end module
