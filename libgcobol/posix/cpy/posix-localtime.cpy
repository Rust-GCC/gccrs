       >>PUSH SOURCE FORMAT
       >>SOURCE FIXED
       identification division.
       function-id. posix-localtime prototype.
       data division.
       linkage section.
       77 return-value usage binary-long.
       01 lk-timep usage pointer.
       01 lk-tm.
       copy tm.

       procedure division using by value lk-timep
                                by reference lk-tm
                                returning return-value.

       end function posix-localtime.

       identification division.
       program-id. posix_localtime prototype.
       data division.
       linkage section.
       77 return-value usage binary-long.
       01 lk-timep usage pointer.
       01 bufsize Usage Binary-Long.

       procedure division using by value lk-timep
                                by value bufsize
                                returning return-value.

       end program posix_localtime.



       >>POP SOURCE FORMAT
