       *> { dg-do run }
       *> { dg-output-file "group2/MOVE_NumericDisplay_to_COMP-3.out" }
        identification              division.
        program-id.                 prog.
        data                        division.
        working-storage             section.
        01 var11    pic     9     value 1     .
        01 var12    pic     99    value 12    .
        01 var13    pic     999   value 123   .
        01 var14    pic     9999  value 1234  .
        01 var1t    pic     999v999 COMP-3.
        01 var21    pic        s9v9  value   -1.2  .
        01 var22    pic       s99v9  value   -12.3  .
        01 var23    pic      s999v9  value   -123.4  .
        01 var24    pic     s9999v9  value   -1234.5  .
        01 var2t    pic      s999v999 COMP-3.
        procedure                   division.
            move var11 to var1t display var1t
            move var12 to var1t display var1t
            move var13 to var1t display var1t
            move var14 to var1t display var1t
            move var21 to var2t display var2t
            move var22 to var2t display var2t
            move var23 to var2t display var2t
            move var24 to var2t display var2t
            move var11 to var2t display var2t
            move var12 to var2t display var2t
            move var13 to var2t display var2t
            move var14 to var2t display var2t
            goback.
        end program                 prog.

