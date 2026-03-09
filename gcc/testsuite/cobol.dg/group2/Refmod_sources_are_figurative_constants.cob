       *> { dg-do run }
       *> { dg-output-file "group2/Refmod_sources_are_figurative_constants.out" }
        id division.
        program-id. prog.
        data division.
        working-storage section.
        01 varx pic x(8) value '""""""""'.
        01 hexed  pic x(32).
        procedure division.
        move "12345678" to varx
        display  """" varx """"
        move "999" to varx(4:3)
        display  """" varx """"
        move zero to varx(4:3)
        display  """" varx """"
        move space to varx(4:3)
        display  """" varx """"
        move quote to varx(4:3)
        display  """" varx """"
        move high-value to varx
        move low-value to varx(4:3)
        move function hex-of(varx) to hexed
        display "low-value  " with no advancing
        evaluate function byte-length("a")
            when 1 if hexed = "FFFFFF000000FFFF" display "Okay"
                    else display "no good" end-if
            when 2 if hexed = "FF00FF00FF00000000000000FF00FF00"
                         display "Okay"
                    else display "no good" end-if
            when 4 if hexed = "to be determined" display "Okay"
                    else display "no good" end-if
            end-evaluate
        move low-value to varx
        move high-value to varx(4:3)
        move function hex-of(varx) to hexed
        display "high-value " with no advancing
        evaluate function byte-length("a")
            when 1 if hexed = "000000FFFFFF0000" display "Okay"
                    else display "no good" end-if
            when 2 if hexed = "000000000000FF00FF00FF0000000000"
                         display "Okay"
                    else display "no good" end-if
            when 4 if hexed = "to be determined" display "Okay"
                    else display "no good" end-if
            end-evaluate
        goback.
        end program prog.

