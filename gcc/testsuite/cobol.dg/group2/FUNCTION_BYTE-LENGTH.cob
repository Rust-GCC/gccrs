       *> { dg-do run }
       *> { dg-output-file "group2/FUNCTION_BYTE-LENGTH.out" }
        identification   division.
        program-id.      prog.
        data             division.
        working-storage  section.
        01  one-char pic x.
        01  x   pic      x(4).
        01  test-fld     pic s9(04)v9(08).
        01  stride usage binary-long.
        01  nbytes usage binary-long.
        procedure        division.
            *> We are going to use this routine to compensate for itself, so
            *> that it can be used for single- and multi=byte encodings:
            move function byte-length(one-char) to stride
            compute test-fld = function byte-length ( test-fld ) / stride
            display "byte-length of pic s9(04)v9(08) is " test-fld
            compute test-fld = function byte-length ( x ) / stride
            display "byte-length of pic x(4) is         "       test-fld
            compute test-fld = function byte-length ( '00128' ) / stride
            display "byte-length of '00128' is          "    test-fld
            move function byte-length ( x'a0' )      to test-fld
            display "byte-length of pic x'a0' is        "      test-fld
            goback.

