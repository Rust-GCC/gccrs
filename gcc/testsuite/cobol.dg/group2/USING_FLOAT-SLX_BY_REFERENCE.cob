       *> { dg-do run }
       *> { dg-output-file "group2/USING_FLOAT-SLX_BY_REFERENCE.out" }
        identification division.
        program-id.  prog.
        data division.
        working-storage section.
        01 var1 float-short         .
        01 var2 float-long          .
        01 var3 float-extended      .
        01 var1r float-short        .
        01 var2r float-long         .
        01 var3r float-extended     .
        procedure division.
            move 1.234E30 to var1
            move 2.345E300 to var2
            move 3.456E4000 to var3
            display     var1
            call     "rvar1" using by reference var1 returning var1r
            display     var1r
            display     var2
            call     "rvar2" using by reference var2 returning var2r
            display     var2r
            display     var3
            call     "rvar3" using by reference var3 returning var3r
            display     var3r
            goback.
        end program prog.

        identification division.
        program-id. rvar1.
        data division.
        linkage section.
        01  var    float-short.
        01  varr   float-short.
        procedure division using by reference var returning varr.
            move var to varr.
        end program rvar1.

        identification division.
        program-id. rvar2.
        data division.
        linkage section.
        01  var    float-long.
        01  varr   float-long.
        procedure division using by reference var returning varr.
           move var to varr.
        end program rvar2.

        identification division.
        program-id. rvar3.
        data division.
        linkage section.
        01  var    float-extended.
        01  varr   float-extended.
        procedure division using by reference var returning varr.
           move var to varr.
        end program rvar3.

