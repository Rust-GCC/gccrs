       *> { dg-do run }
       *> { dg-output-file "group2/USING_Signed___COMP-3_BY_REFERENCE.out" }
        identification division.
        program-id.  prog.
        data division.
        working-storage section.
        01 var8    pic s9(2) comp-3.
        01 var16   pic s9(4) comp-3.
        01 var32   pic s9(8) comp-3.
        01 var64   pic s9(16) comp-3.
        01 var128  pic s9(32) comp-3.
        01 var8r   pic s9(2) comp-3.
        01 var16r  pic s9(4) comp-3.
        01 var32r  pic s9(8) comp-3.
        01 var64r  pic s9(16) comp-3.
        01 var128r pic s9(32) comp-3.
        procedure division.
            move    12 to var8
            move    1234 to var16
            move    12345678 to var32
            move    1234567890123456 to var64
            move    12345678901234567890123456789012 to var128
            display     var8
            call     "rvar8" using by reference var8 returning var8r
            display     var8r
            display     var16
            call     "rvar16" using by reference var16 returning var16r
            display     var16r
            display     var32
            call     "rvar32" using by reference var32 returning var32r
            display     var32r
            display     var64
            call     "rvar64" using by reference var64 returning var64r
            display     var64r
            display     var128
            call     "rvar128" using by reference var128 returning var128r
            display     var128r
            goback.
        end program prog.

        identification division.
        program-id. rvar8.
        data division.
        linkage section.
        01  var         pic 9(2) comp-3               .
        01  varr        pic 9(2) comp-3               .
        procedure division using by reference var returning varr.
            move var to varr.
        end program rvar8.

        identification division.
        program-id. rvar16.
        data division.
        linkage section.
        01  var         pic 9(4) comp-3               .
        01  varr        pic 9(4) comp-3               .
        procedure division using by reference var returning varr.
            move var to varr.
        end program rvar16.

        identification division.
        program-id. rvar32.
        data division.
        linkage section.
        01  var         pic 9(8) comp-3               .
        01  varr        pic 9(8) comp-3               .
        procedure division using by reference var returning varr.
            move var to varr.
        end program rvar32.

        identification division.
        program-id. rvar64.
        data division.
        linkage section.
        01  var         pic 9(16) comp-3               .
        01  varr        pic 9(16) comp-3               .
        procedure division using by reference var returning varr.
            move var to varr.
        end program rvar64.

        identification division.
        program-id. rvar128.
        data division.
        linkage section.
        01  var         pic 9(32) comp-3               .
        01  varr        pic 9(32) comp-3               .
        procedure division using by reference var returning varr.
           move var to varr.
        end program rvar128.

