       *> { dg-do run }
       *> { dg-output-file "group2/COMP-3_to_COMP-3__IN-PHASE__MOVES.out" }
        identification              division.
        program-id.                 prog.
        data                        division.
        working-storage             section.
        *> Test MOVE COMP-3 to COMP-3 where the source and destination are
        *> "in-phase" with regard to decimal-point location.
        *> Same-same
        01 var11 pic   999v99 comp-3 value  123.45 .
        01 var12 pic  s999v99 comp-3 value  123.45 .
        01 var13 pic  s999v99 comp-3 value -123.45 .
        01 var14 pic   999v9999 comp-3.
        01 var15 pic  s999v9999 comp-3.
        01 var16 pic  s999v9999 comp-3.
        *>big-small to small-big 
        01 var21 pic   9999v99 comp-3 value  1234.56 .
        01 var22 pic  s9999v99 comp-3 value  1234.56 .
        01 var23 pic  s9999v99 comp-3 value -1234.56 .
        01 var24 pic   999v9999 comp-3.
        01 var25 pic  s999v9999 comp-3.
        01 var26 pic  s999v9999 comp-3.
        *>small-big to big-small 
        01 var31 pic    999v9999 comp-3 value  123.4567 .
        01 var32 pic   s999v9999 comp-3 value  123.4567 .
        01 var33 pic   s999v9999 comp-3 value -123.4567 .
        01 var34 pic   9999v99 comp-3.
        01 var35 pic  s9999v99 comp-3.
        01 var36 pic  s9999v99 comp-3.
        procedure                   division.
           display "same-same"
           move var11 to var14 var15 var16
            display space var14 space "should be  123.4500"
            display       var15 space "should be +123.4500"
            display       var16 space "should be +123.4500"
           move var12 to var14 var15 var16
            display space var14 space "should be  123.4500"
            display       var15 space "should be +123.4500"
            display       var16 space "should be +123.4500"
           move var13 to var14 var15 var16
            display space var14 space "should be  123.4500"
            display       var15 space "should be -123.4500"
            display       var16 space "should be -123.4500"
           display "big-small to small-big"
           move var21 to var24 var25 var26
            display space var24 space "should be  234.5600"
            display       var25 space "should be +234.5600"
            display       var26 space "should be +234.5600"
           move var22 to var24 var25 var26
            display space var24 space "should be  234.5600"
            display       var25 space "should be +234.5600"
            display       var26 space "should be +234.5600"
           move var23 to var24 var25 var26
            display space var24 space "should be  234.5600"
            display       var25 space "should be -234.5600"
            display       var26 space "should be -234.5600"
           display "small-big to big-small"
           move var31 to var34 var35 var36
            display space var34 space "should be  0123.4500"
            display       var35 space "should be +0123.4500"
            display       var36 space "should be +0123.4500"
           move var32 to var34 var35 var36
            display space var34 space "should be  0123.4500"
            display       var35 space "should be +0123.4500"
            display       var36 space "should be +0123.4500"
           move var33 to var34 var35 var36
            display space var34 space "should be  0123.4500"
            display       var35 space "should be -0123.4500"
            display       var36 space "should be -0123.4500"
            goback.
        end program                 prog.

