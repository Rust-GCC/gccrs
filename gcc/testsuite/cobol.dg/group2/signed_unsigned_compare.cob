       *> { dg-do run }
       *> { dg-output-file "group2/signed_unsigned_compare.out" }
        identification          division.
        program-id.             prog.
        data                    division.
        working-storage         section.
        01 aaa binary-long   signed value -1.
        01 bbb binary-long unsigned value  1.
        procedure               division.
        if aaa < bbb
            display "-1 is properly .LT. than +1"
        else
            display "-1 is IMPROPERLY .GE. than +1"
            end-if
        goback.
        end program             prog.

