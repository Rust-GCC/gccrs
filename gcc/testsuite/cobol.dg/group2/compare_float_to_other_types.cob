       *> { dg-do run }
       *> { dg-output-file "group2/compare_float_to_other_types.out" }
        identification          division.
        program-id.             prog.
        data                    division.
        working-storage         section.
        *> This is the beginning of a larger sanity check for comparing
        *> values.  I ran out of enthusiasm today.
        77 var01    pic     9999     DISPLAY          value 1000     .
        77 var02    pic     9ppp     DISPLAY          value 1000     .
        77 var03    pic     9999v99  DISPLAY          value 1000     .
        77 var04    pic     v999999  DISPLAY          value 0.000100 .
        77 var05    pic     ppp999   DISPLAY          value 0.000100 .

        77 var31    pic     9999     COMP-3          value 1000     .
        77 var32    pic     9ppp     COMP-3          value 1000     .
        77 var33    pic     9999v99  COMP-3          value 1000     .
        77 var34    pic     v999999  COMP-3          value 0.000100 .
        77 var35    pic     ppp999   COMP-3          value 0.000100 .

        77 var41    pic     9999     COMP-4          value 1000     .
        77 var42    pic     9ppp     COMP-4          value 1000     .
        77 var43    pic     9999v99  COMP-4          value 1000     .
        77 var44    pic     v999999  COMP-4          value 0.000100 .
        77 var45    pic     ppp999   COMP-4          value 0.000100 .

        77 var51    pic     9999     COMP-5          value 1000     .
        77 var52    pic     9ppp     COMP-5          value 1000     .
        77 var53    pic     9999v99  COMP-5          value 1000     .
        77 var54    pic     v999999  COMP-5          value 0.000100 .
        77 var55    pic     ppp999   COMP-5          value 0.000100 .

        77 var61    pic     9999     packed-decimal no sign    value 1000     .
        77 var62    pic     9ppp     packed-decimal no sign    value 1000     .
        77 var63    pic     9999v99  packed-decimal no sign    value 1000     .
        77 var64    pic     v999999  packed-decimal no sign    value 0.000100 .
        77 var65    pic     ppp999   packed-decimal no sign    value 0.000100 .

        77 f1     float-long value 1500.
        77 f2     float-long value 0.000200 .

        procedure               division.
            if f1 > var01 display "01okay" else display "01BAD" end-if
            if f1 > var31 display "03okay" else display "03BAD" end-if
            if f1 > var41 display "04okay" else display "04BAD" end-if
            if f1 > var51 display "05okay" else display "05BAD" end-if
            if f1 > var61 display "06okay" else display "05BAD" end-if

            if f1 < var01 display "01BAD~" else display "01okay~" end-if
            if f1 < var31 display "03BAD~" else display "03okay~" end-if
            if f1 < var41 display "04BAD~" else display "04okay~" end-if
            if f1 < var51 display "05BAD~" else display "05okay~" end-if
            if f1 < var61 display "06BAD~" else display "06okay~" end-if
            goback.
        end program             prog.

