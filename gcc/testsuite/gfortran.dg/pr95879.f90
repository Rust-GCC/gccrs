! { dg-do compile }
! { dg-options "-std=f2018" }
! PR fortran/95879
! Contributed by G. Steinmetz <gscfq@t-online.de>
!
! Use-after-free in gfc_resolve_formal_arglist when a contained subroutine
! has a statement function whose dummy argument name matches a symbol
! replaced by gfc_fixup_sibling_symbols.

module m1
contains
   integer function f(x) bind(c)
      use iso_c_binding
   contains
      subroutine s
         c_funloc(f) = x  ! { dg-warning "Obsolescent feature: Statement function" }
      end
   end
end

module m2
contains
   integer function f(x)
      use iso_c_binding
   contains
      subroutine s
         c_funloc(f) = x  ! { dg-warning "Obsolescent feature: Statement function" }
      end
   end
end
