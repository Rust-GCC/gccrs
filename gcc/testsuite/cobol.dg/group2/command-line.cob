       *> { dg-do run }
       *> { dg-output-file "group2/command-line.out" }

       *> ODD FAILURE: failing to recognize "" as SPACE
       identification division.
       program-id. prog.
       data division.
       working-storage section.
       77 cmd-line-parm pic x(20).
       procedure division.
          ACCEPT cmd-line-parm FROM COMMAND-LINE(2).
          IF cmd-line-parm NOT EQUAL SPACE THEN
             DISPLAY "Not SPACE: " """" cmd-line-parm """"
          ELSE
             DISPLAY "Okay"
             END-IF.
           end program prog.

