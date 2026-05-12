       *> { dg-do run }
       *> { dg-output-file "group2/Large_PIC_10000000_.out" }
        identification   division.
        program-id.      prog.
        data             division.
        working-storage  section.
        01 uppered       pic x(10000000) value all "A".
        01 lowered       pic x(10000000).
        procedure        division.
            move function lower-case(uppered) to lowered
            move 'X' to lowered(1:1)
            move 'Z' to lowered(10000000:1)
            display lowered(1:10)
            display lowered(9999991:10)
            goback.
        end program     prog.

