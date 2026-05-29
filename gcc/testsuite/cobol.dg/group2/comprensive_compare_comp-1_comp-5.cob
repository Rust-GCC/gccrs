       *> { dg-do run }
       *> { dg-output-file "group2/comprensive_compare_comp-1_comp-5.out" }
        identification          division.
        program-id.             prog.
        data                    division.
        working-storage         section.
        01 aaa comp-1.
        01 bbb pic S999 comp-5.
        01 known pic xxxx.
        01 result pic xxxx.
        procedure               division.
            move -1 to aaa move 1 to bbb move ".lt." to known
            perform checker.
            move  1 to aaa move 1 to bbb move ".eq." to known
            perform checker.
            move  1 to aaa move -1 to bbb move ".gt." to known
            perform checker.
            goback.
        checker.
            display "checking " space aaa space known space bbb
            perform lt
            perform le
            perform eq
            perform ge
            perform gt
            perform ne
            continue.
        lt.
            display "  .lt. " with no advancing
            move "xxxx" to result
            evaluate true
              when known equal ".lt." if aaa < bbb move "Good" to result
                                       else move "BAD!" to result end-if
              when known equal ".eq." if aaa < bbb move "BAD!" to result
                                      else move "Good" to result end-if
              when known equal ".gt." if aaa < bbb move "BAD!" to result
                                      else move "Good" to result end-if
            end-evaluate
            display space result
            continue.
        le.
            display "  .le. " with no advancing
            move "xxxx" to result
            evaluate true
              when known equal ".lt." if aaa <= bbb
                 move "Good" to result else move "BAD!" to result end-if
              when known equal ".eq." if aaa <= bbb
                 move "Good" to result else move "BAD!" to result end-if
              when known equal ".gt." if aaa <= bbb
                 move "BAD!" to result else move "Good" to result end-if
            end-evaluate
            display space result
            continue.
        eq.
            display "  .eq. " with no advancing
            move "xxxx" to result
            evaluate true
              when known equal ".lt." if aaa = bbb
                 move "BAD!" to result else move "Good" to result end-if
              when known equal ".eq." if aaa = bbb
                 move "Good" to result else move "BAD!" to result end-if
              when known equal ".gt." if aaa = bbb
                 move "BAD!" to result else move "Good" to result end-if
            end-evaluate
            display space result
            continue.
        ge.
            display "  .ge. " with no advancing
            move "xxxx" to result
            evaluate true
              when known equal ".lt." if aaa >= bbb
                move "BAD!" to result else move "Good" to result end-if
              when known equal ".eq." if aaa >= bbb
                move "Good" to result else move "BAD!" to result end-if
              when known equal ".gt." if aaa >= bbb
                move "Good" to result else move "BAD!" to result end-if
            end-evaluate
            display space result
            continue.
        gt.
            display "  .gt. " with no advancing
            move "xxxx" to result
            evaluate true
              when known equal ".lt." if aaa > bbb
                move "BAD!" to result else move "Good" to result end-if
              when known equal ".eq." if aaa > bbb
                move "BAD!" to result else move "Good" to result end-if
              when known equal ".gt." if aaa > bbb
                move "Good" to result else move "BAD!" to result end-if
            end-evaluate
            display space result
            continue.
        ne.
            display "  .ne. " with no advancing
            move "xxxx" to result
            evaluate true
              when known equal ".lt." if aaa <> bbb
                move "Good" to result else move "BAD!" to result end-if
              when known equal ".eq." if aaa <> bbb
                move "BAD!" to result else move "Good" to result end-if
              when known equal ".gt." if aaa <> bbb
                move "Good" to result else move "BAD!" to result end-if
            end-evaluate
            display space result
            continue.
        end program             prog.

