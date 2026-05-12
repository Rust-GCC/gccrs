       *> { dg-do run }
       *> { dg-output-file "group2/PERFORM_TIMES_subscripted.out" }
        identification      division.
        program-id.         prog.
        data                division.
        working-storage section.
        01  idx                       pic 9.
        01  cnt-tab.
            05 cnt-val occurs 3 times pic 9.
        procedure division.
            move 1 to cnt-val (1)
            move 2 to cnt-val (2)
            move 3 to cnt-val (3)
            perform varying idx from 1 by 1 until idx > 3
                perform cnt-val (idx) times
                    display idx
                    end-perform
                end-perform
            goback.
        end program         prog.

