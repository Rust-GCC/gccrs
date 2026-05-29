       >>PUSH SOURCE FORMAT
       >>SOURCE FIXED
       identification division.
       function-id. posix-fstat prototype.
       data division.
       linkage section.
       77 retcode binary-long.
       01 file-handle pic 9(8) usage comp.
       01 statbuf.
        COPY statbuf.
       procedure division using
                          by value file-handle,
                          by reference statbuf
                          returning retcode.
       end function posix-fstat.
       >> POP SOURCE FORMAT
