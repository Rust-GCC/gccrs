       >>PUSH SOURCE FORMAT
       >>SOURCE FIXED
       identification division.
       function-id. posix-mkdir prototype.
       data division.
       linkage section.
       77 return-value binary-long.
       01 lk-pathname pic x any length.
       01 lk-mode binary-long.

       procedure division using by reference lk-pathname
                                by value lk-mode
                                returning return-value.

       end function posix-mkdir.
       >> POP SOURCE FORMAT
