       *> { dg-do run }
       *> { dg-output-file "group2/CDF__2__IF____number_.out" }

       >>DEFINE FOO AS 1
       id division.
       program-id. prog.
       procedure division.
           >>IF FOO = 1
           DISPLAY "                 FOO is 1.".
           >>END-IF
           DISPLAY "Should have seen FOO is 1.".
           >>IF FOO = 2
           DISPLAY "                 FOO is 2.".
           >>END-IF
           DISPLAY "Shouldn't see    FOO is 2.".
       goback.

