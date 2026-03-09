       *> { dg-do run }
       *> { dg-output-file "group2/CDF__3__ALL_NUMERIC_COMPARISONS.out" }

        >>DEFINE ONE AS 1
        >>DEFINE TWO AS 2
        >>DEFINE WUN AS 1
        id division.
        program-id. prog.
        procedure division.
            >>IF ONE = TWO
            DISPLAY "??? ONE =  TWO ???"
            >>END-IF
            >>IF ONE <> TWO
            DISPLAY "ONE <> TWO"
            >>END-IF
            >>IF ONE < TWO
            DISPLAY "ONE <  TWO"
            >>END-IF
            >>IF ONE <= TWO
            DISPLAY "ONE <= TWO"
            >>END-IF
            >>IF ONE >= TWO
            DISPLAY "??? ONE >= TWO ???"
            >>END-IF
            >>IF ONE > TWO
            DISPLAY "??? ONE > TWO ???"
            >>END-IF
            >>IF ONE = WUN
            DISPLAY "ONE =  ONE"
            >>END-IF
            >>IF ONE <> WUN
            DISPLAY "??? ONE <> ONE ???"
            >>END-IF
            >>IF ONE < WUN
            DISPLAY "??? ONE <  ONE ???"
            >>END-IF
            >>IF ONE <= WUN
            DISPLAY "ONE <= ONE"
            >>END-IF
            >>IF ONE >= WUN
            DISPLAY "ONE >= ONE"
            >>END-IF
            >>IF ONE > WUN
            DISPLAY "??? ONE > ONE ???"
            >>END-IF
        goback.

