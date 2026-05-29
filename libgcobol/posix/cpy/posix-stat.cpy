       >>PUSH SOURCE FORMAT
       >>SOURCE FIXED
       identification division.
       function-id. posix-stat prototype.
       data division.
       linkage section.
       77 return-value binary-long.
       01 lk-pathname pic x any length.
       01 lk-statbuf.
       COPY statbuf.
       procedure division using by reference lk-pathname
                                by reference lk-statbuf
                                returning return-value.
       end function posix-stat.
       >> POP SOURCE FORMAT
