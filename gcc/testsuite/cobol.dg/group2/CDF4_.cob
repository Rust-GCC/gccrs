       *> { dg-do run }
       *> { dg-output-file "group2/CDF4_.out" }

       >>DEFINE FOO AS 1
      *> Only the gratuitous display message shows. The message
      *> enclosed in the IF does not.
       id division.
       program-id. prog.
       procedure division.
           >>IF FOO = 1
           DISPLAY "FOO is one.".
           >>END-IF
           DISPLAY "gratuitous display.".
       goback.

