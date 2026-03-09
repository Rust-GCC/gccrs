       *> { dg-do run }
       *> { dg-output-file "group2/ACCEPT_FROM_ENVIRONMENT-NAME.out" }
        identification division.
        program-id. wrapper.
        data division.
        working-storage section.
        01 ename        pic x(32).
        01 evalue       pic x(32).
        procedure division.
        move "BAGPIPES" to ename
        display ename  upon environment-name.
        accept  evalue from environment-value 
                on exception display "No "  function trim (ename) end-display
            not on exception display "Got " function trim (ename) end-display
            end-accept

        display ename  upon environment-name.
        display "loud" upon environment-value.

        display ename  upon environment-name.
        accept  evalue from environment-value 
                on exception display "No "  function trim (ename) end-display
            not on exception display "Got " function trim (ename) end-display
            end-accept
        display """" function trim(evalue) """"

        goback.
        end program wrapper.

