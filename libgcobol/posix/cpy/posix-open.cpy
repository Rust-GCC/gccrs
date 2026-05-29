       >>PUSH SOURCE FORMAT
       >>SOURCE FIXED
       identification division.
       function-id. posix-open prototype.
       data division.
       Linkage Section.
          77 Return-Value Binary-Long Signed.
          01 Lk-pathname PIC X ANY LENGTH.
          01 Lk-flags    PIC 9(8) comp-5.
          01 Lk-mode     PIC 9(8) comp-5.

       Procedure Division using
             By Reference Lk-pathname,
             By Value Lk-flags,
             By Value Lk-mode
             Returning Return-Value.

       end function posix-open.
       >> POP SOURCE FORMAT
