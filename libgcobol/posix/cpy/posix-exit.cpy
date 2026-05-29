       >>PUSH SOURCE FORMAT
       >>SOURCE FIXED
       identification division.
       function-id. posix-exit prototype.
       data division.
       linkage section.
       77 return-value binary-long.
       77 exit-status binary-long.
       procedure division using exit-status returning return-value.
       end function posix-exit.
       >> POP SOURCE FORMAT
