! { dg-do compile }
! { dg-options "-fc-prototypes" }
!
! PR fortran/125902
!
! Test -fc-prototypes for assumed length character dummy, header include

subroutine sub (b) bind(C)
  character(*) :: b
end subroutine sub

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

void sub (CFI_cdesc_t *b);

#ifdef __cplusplus
}
#endif
!{ dg-end-multiline-output "" }
#endif
