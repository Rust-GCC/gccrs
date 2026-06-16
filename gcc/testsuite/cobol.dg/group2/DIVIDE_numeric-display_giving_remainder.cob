       *> { dg-do run }
       *> { dg-output-file "group2/DIVIDE_numeric-display_giving_remainder.out" }
        identification              division.
        program-id.                 prog.
        data                        division.
        working-storage             section.
        01 var1     pic s999.
        01 var2     pic s999.
        01 Q        pic s999.
        01 r        pic s999.
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

