       *> { dg-do run }
       *> { dg-output-file "group2/FUNCTION_ORD.out" }
        identification   division.
        program-id.      prog.
        data             division.
        working-storage  section.
        01 result pic 999.
        procedure        division.
            move function ord ( "k" ) to result
            if result = 147 or 108
                display "ORD result is ebcdic or ascii for 'k'"
            else
                display "ORD result is improperly " result
            end-if
            goback.
        end program     prog.

