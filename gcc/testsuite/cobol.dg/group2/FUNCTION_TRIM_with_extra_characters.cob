       *> { dg-do run }
       *> { dg-output-file "group2/FUNCTION_TRIM_with_extra_characters.out" }
        identification              division.
        program-id.                 prog.
        data                        division.
        working-storage             section.
        77 foo pic x(24).
        77 b pic x value 'b'.
        procedure                   division.
            move '  """000Bobaaabbbccc     ' to foo
            display ''''
                 function trim(foo space quote zero 'c' b 'a')
                    ''''
            display ''''
                 function trim(foo leading space quote zero 'c' b 'a')
                    ''''
            display ''''
                 function trim(foo trailing space quote zero 'c' b 'a')
                    ''''
            goback.
        end program                 prog.

