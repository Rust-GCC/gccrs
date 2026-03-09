       *> { dg-do run }
       *> { dg-output-file "group2/MOVE_X_000203_.out" }
        identification   division.
        program-id.      prog.
        data             division.
        working-storage  section.
        01 x             pic xxx.
        01 stride        binary-long.
        01 hexed         pic x(24).
        procedure        division.
            move function byte-length('a') to stride.
            evaluate stride
                when 1 move x"000203" to x
                when 2 move x"000203040506" to x
                when 4 move x"0002030405060708090a0b0c" to x
                end-evaluate
           move function hex-of(x) to hexed
            evaluate stride
                when 1 if hexed = "000203" 
                        display "Okay" else display "1 BAD" end-if
                when 2 if hexed = "000203040506" 
                        display "Okay" else display "2 BAD" end-if
                when 4 if hexed = "0002030405060708090a0b0c"
                        display "Okay" else display "4 BAD" end-if
                end-evaluate
           goback.

