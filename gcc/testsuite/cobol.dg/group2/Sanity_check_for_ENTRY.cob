       *> { dg-do run }
       *> { dg-output-file "group2/Sanity_check_for_ENTRY.out" }
        identification   division.
        program-id.      prog.
        data             division.
        working-storage  section.
        01 foo pic x(12).
        procedure        division.
        loop.
            go to dispatch.
        dispatch.
            go to pass_1.
        pass_1.
            display "I am the first pass"
            alter dispatch to pass_2.
            go to loop.
        pass_2.
            display "I am the second pass"
            alter dispatch to pass_3.
            go to loop.
        pass_3.
            display "I am the third and final pass"
            goback.
        end program     prog.

