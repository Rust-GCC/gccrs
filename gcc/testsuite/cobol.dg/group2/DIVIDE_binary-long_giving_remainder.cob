       *> { dg-do run }
       *> { dg-output-file "group2/DIVIDE_binary-long_giving_remainder.out" }
        identification              division.
        program-id.                 prog.
        data                        division.
        working-storage             section.
        01 var1     binary-long.
        01 var2     binary-long.
        01 Q        binary-long.
        01 r        binary-long.
        procedure                   division.
            move  11 to var1 move  2 to var2
                divide var1 by var2 giving Q remainder R
                display Q space R
            move -11 to var1 move  2 to var2
                divide var1 by var2 giving Q remainder R
                display Q space R
            move  11 to var1 move -2 to var2
                divide var1 by var2 giving Q remainder R
                display Q space R
            move -11 to var1 move -2 to var2
                divide var1 by var2 giving Q remainder R
                display Q space R
            goback.
        end program                 prog.

