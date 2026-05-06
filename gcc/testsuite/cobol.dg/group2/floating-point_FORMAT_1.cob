       *> { dg-do run }
       *> { dg-output-file "group2/floating-point_FORMAT_1.out" }
       identification division.
       program-id. prog.

       data division.
       working-storage section.
       01  cmp1a                        comp-1.
       01  cmp1b                        comp-1.

       01  cmp2a                        comp-2.
       01  cmp2b                        comp-2.

       01  cmp3a                        float-extended.
       01  cmp3b                        float-extended.

       procedure division.
            display "--- COMP-1 FORMAT 1 ---"
            move 10 to cmp1a
            move  7 to cmp1b
            add      cmp1b to    cmp1a 
            display  "add      " cmp1a
            move 10 to cmp1a
            subtract cmp1b from  cmp1a 
            display  "subtract " cmp1a
            move 10 to cmp1a
            multiply cmp1b by    cmp1a 
            display  "multiply " cmp1a
            move 10 to cmp1a
            divide   cmp1b into    cmp1a 
            display  "divide   " cmp1a

            display "--- COMP-2 FORMAT 1 ---"
            move 10 to cmp2a
            move  7 to cmp2b
            add      cmp2b to    cmp2a 
            display  "add      " cmp2a
            move 10 to cmp2a
            subtract cmp2b from  cmp2a 
            display  "subtract " cmp2a
            move 10 to cmp2a
            multiply cmp2b by    cmp2a 
            display  "multiply " cmp2a
            move 10 to cmp2a
            divide   cmp1b into    cmp2a 
            display  "divide   " cmp2a

            display "--- FLOAT-EXTENDED FORMAT 1 ---"
            move 10 to cmp3a
            move  7 to cmp3b
            add      cmp3b to    cmp3a 
            display  "add      " cmp3a
            move 10 to cmp3a
            subtract cmp3b from  cmp3a 
            display  "subtract " cmp3a
            move 10 to cmp3a
            multiply cmp3b by    cmp3a 
            display  "multiply " cmp3a
            move 10 to cmp3a
            divide   cmp1b into    cmp3a 
            display  "divide   " cmp3a

           goback.
       end program prog.

