       *> { dg-do run }
       *> { dg-options "-dialect mf" }
       *> { dg-output-file "group2/MOVE_LEVEL_78.out" }
        identification   division.
        program-id. prog.
        data division.
        working-storage section.
        78 constq value quotes.
        78 consts value spaces.
        78 constz value zeroes.
        78 constl value low-values.
        78 consth value high-values.
        01 str pic x(10).
        01 strp redefines str pointer.
        01 s  pic x(8) value Space.
        01 sp redefines s pointer.
        01 q pic x(8) value Quote.
        01 qp redefines q pointer.
        01 z pic x(8) value Zero.
        01 zp redefines z pointer.
        procedure        division.
            move constl to str
            display strp
            move consts to str
            if strp = sp
                display "Space OK."
            else
                display "Space no good."
                end-if
            move constq to str
            if strp = qp
                display "Quote OK."
            else
                display "Quote no good: " '"' qp '"' ' <> ' '"' strp  '"'
                end-if
            move constz to str
            if strp = zp
                display "Zero OK."
            else
                display "Zero no good:  " '"' zp '"' ' <> ' '"' strp  '"'
                end-if
            move consth to str
            if str equal consth
                display "High-value OK."
            else
                display "High-value is no good: "
                         '"' function hex-of(consth) '"' ' <> ' '"' function hex-of(str)  '"'
            goback.
        end program prog.


