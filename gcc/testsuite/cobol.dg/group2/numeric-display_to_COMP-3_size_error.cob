       *> { dg-do run }
       *> { dg-output-file "group2/numeric-display_to_COMP-3_size_error.out" }
        identification division.
        program-id. onsize.
        data division.
        working-storage section.
        01 var11 pic  99999 display value  12345.
        01 var12 pic 999999 display value 123456.
        01 var13 pic  999   comp-3              .
        01 var14 pic 9999   comp-3              .
        procedure       division.
            display "test1: " with no advancing
            compute var13 = var11
                ON SIZE ERROR Display "Proper size error"
                NOT ON SIZE ERROR Display "IMPROPER no error"
                end-compute
            display "test2: " with no advancing
            compute var14 = var11
                ON SIZE ERROR Display "Proper size error"
                NOT ON SIZE ERROR Display "IMPROPER no error"
                end-compute
            display "test3: " with no advancing
            compute var13 = var12
                ON SIZE ERROR Display "Proper size error"
                NOT ON SIZE ERROR Display "IMPROPER no error"
                end-compute
            display "test4: " with no advancing
            compute var14 = var12
                ON SIZE ERROR Display "Proper size error"
                NOT ON SIZE ERROR Display "IMPROPER no error"
                end-compute
            goback.
        end program onsize.

