       >>PUSH SOURCE FORMAT
       >>SOURCE FIXED
       identification division.
       function-id. posix-ftruncate prototype.
       data division.
       linkage section.
       77 return-value binary-long.
       01 lk-fd pic 9(8) usage comp.
       01 lk-offset binary-double.
       procedure division using by value lk-fd
                                by value lk-offset
                                returning return-value.
       end function posix-ftruncate.
       >>POP SOURCE FORMAT
