       *> { dg-do run }
       *> { dg-output-file "group2/Clear_negative_zero_after_truncated_MOVE.out" }
        identification division.
        program-id. onsize.
        data division.
        working-storage section.
        01 var11 pic s9999 value -1000.
        01 var12 pic s999.
        01 var21 pic s9999 comp-3 value -1000.
        01 var22 pic s999  comp-3 .
        procedure       division.
            *> Make sure we don't create "-0000"
            move    var21 to var22
            display          var22 space function hex-of(var22)
            move    var11 to var22
            display          var22 space function hex-of(var22)
            move    var11 to var12
            display          var12 space ''''var12(3:1)''''
            move    var21 to var12
            display          var12 space ''''var12(3:1)''''
            goback.
        end program onsize.

