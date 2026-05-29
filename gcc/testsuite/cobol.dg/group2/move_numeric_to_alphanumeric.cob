       *> { dg-do run }
       *> { dg-output-file "group2/move_numeric_to_alphanumeric.out" }
        identification      division.
        program-id.         prog.
        data                division.
        working-storage     section.
        01 aaa pic s9.
        01 bbb pic s999ppp.
        01 xxx pic x(16).
        procedure           division.
            display "This should be 001"
            move 001 to xxx display xxx
            display "These should be 1"
            move  -1 to xxx display xxx
            move  +1 to xxx display xxx
            move   1 to xxx display xxx
            move   1 to aaa move aaa to xxx display xxx
            move  -1 to aaa move aaa to xxx display xxx
            display "These should be 001000"
            move  1000 to bbb move bbb to xxx display xxx
            move -1000 to bbb move bbb to xxx display xxx
            goback.
        end program         prog.

