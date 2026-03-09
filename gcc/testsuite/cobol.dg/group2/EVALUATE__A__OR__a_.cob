       *> { dg-do run }
       *> { dg-output-file "group2/EVALUATE__A__OR__a_.out" }
        program-id. prog.
        data division.
        working-storage section.
        77 answer pic x.
        procedure division.
        move 'a' to answer
        evaluate answer
        when = "A" or "a"
            display "answer is '" answer "'"
        when other
            display answer"  is neither 'A' nor 'a' " 
        end-evaluate.

        move 'A' to answer
        evaluate answer
        when = "A" or "a"
            display "answer is '" answer "'"
        when other
            display answer"  is neither 'A' nor 'a' " 
        end-evaluate.
        end program prog.

