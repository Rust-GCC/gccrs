! { dg-do compile }
! { dg-options "-fc-prototypes" }
!
! PR fortran/125902
!
! Test -fc-prototypes for deferred shape and deferred length dummies

module test_c_prototypes_module

  use iso_c_binding
  implicit none (type, external)

  interface
    subroutine testsub1(b, c, d, r, ier) bind(C)
      import
      character(kind=c_char,len=*),              intent(in)    :: b
      real(c_float),  dimension(:),              intent(inout) :: c
      real(c_double), dimension(:), allocatable, intent(inout) :: d
      character(kind=c_char,len=:), allocatable, intent(inout) :: r
      integer(c_int),                            intent(out)   :: ier
    end subroutine testsub1
  end interface

  interface
    subroutine testsub2(b, msg, c, d, r) bind(C)
      import
      character(kind=c_char),                intent(in)    :: b(:)
      character(kind=c_char),                intent(out)   :: msg(*)
      real(c_float),  dimension(*),          intent(inout) :: c
      real(c_double), dimension(:), pointer, intent(inout) :: d
      character(kind=c_char,len=:), pointer, intent(inout) :: r
    end subroutine testsub2
  end interface

end module test_c_prototypes_module 

#if 0
!{ dg-begin-multiline-output "" }
#include <stddef.h>
#ifdef __cplusplus
#include <complex>
#define __GFORTRAN_FLOAT_COMPLEX std::complex<float>
#define __GFORTRAN_DOUBLE_COMPLEX std::complex<double>
#define __GFORTRAN_LONG_DOUBLE_COMPLEX std::complex<long double>
extern "C" {
#else
#define __GFORTRAN_FLOAT_COMPLEX float _Complex
#define __GFORTRAN_DOUBLE_COMPLEX double _Complex
#define __GFORTRAN_LONG_DOUBLE_COMPLEX long double _Complex
#endif

#include <ISO_Fortran_binding.h>

void testsub1 (const CFI_cdesc_t *b, CFI_cdesc_t *c, CFI_cdesc_t *d, CFI_cdesc_t *r, int *ier);
void testsub2 (const CFI_cdesc_t *b, char *msg, float *c, CFI_cdesc_t *d, CFI_cdesc_t *r);

#ifdef __cplusplus
}
#endif
!{ dg-end-multiline-output "" }
#endif
