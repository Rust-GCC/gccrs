       *> { dg-do run }
       *> { dg-output-file "group2/COMP-3_to_COMP-3__OUT-OF-PHASE__MOVES.out" }
        identification              division.
        program-id.                 prog.
        data                        division.
        working-storage             section.
        *> Test MOVE COMP-3 to COMP-3 where the source and destination are
        *> "out-of-phase" with regard to decimal-point location.
        01 var11 pic  999v9(1) comp-3 value  123.4 .
        01 var12 pic  999v9(3) comp-3 value  123.456 .
        01 var13 pic  999v9(5) comp-3 value  123.45678 .
        01 var14 pic  999v9(7) comp-3 value  123.4567898 .
        01 var1t pic  999v9999   comp-3.
        01 var21  pic  s999v9999  comp-3 value -123.4567.
        01 var2t1 pic  s999v9(1) comp-3.
        01 var2t2 pic  s999v9(3) comp-3.
        01 var2t3 pic  s999v9(5) comp-3.
        01 var2t4 pic  s999v9(7) comp-3.
        01 var31 pic  9999v9(1) comp-3 value  9123.4 .
        01 var32 pic  9999v9(3) comp-3 value  9123.456 .
        01 var33 pic  9999v9(5) comp-3 value  9123.45678 .
        01 var34 pic  9999v9(7) comp-3 value  9123.4567898 .
        01 var3t pic  999v9999   comp-3.
        01 var41  pic  s9999v9999  comp-3 value -9123.4567.
        01 var4t1 pic  s999v9(1) comp-3.
        01 var4t2 pic  s999v9(3) comp-3.
        01 var4t3 pic  s999v9(5) comp-3.
        01 var4t4 pic  s999v9(7) comp-3.
        procedure                   division.
            move var11 to var1t display var1t " should be 123.4000"
            move var12 to var1t display var1t " should be 123.4560"
            move var13 to var1t display var1t " should be 123.4567"
            move var14 to var1t display var1t " should be 123.4567"
            move var21 to var2t1 var2t2 var2t3 var2t4
            display var2t1 "       should be -123.4"
            display var2t2 "     should be -123.456"
            display var2t3 "   should be -123.45670"
            display var2t4 " should be -123.4567000"
            move var31 to var3t display var3t " should be 123.4000"
            move var32 to var3t display var3t " should be 123.4560"
            move var33 to var3t display var3t " should be 123.4567"
            move var34 to var3t display var3t " should be 123.4567"
            move var41 to var4t1 var4t2 var4t3 var4t4
            display var4t1 "       should be -123.4"
            display var4t2 "     should be -123.456"
            display var4t3 "   should be -123.45670"
            display var4t4 " should be -123.4567000"
            goback.
        end program                 prog.

