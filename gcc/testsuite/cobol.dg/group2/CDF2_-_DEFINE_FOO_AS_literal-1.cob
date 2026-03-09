       *> { dg-do run }
       *> { dg-output-file "group2/CDF2_-_DEFINE_FOO_AS_literal-1.out" }
       >>DEFINE FOO AS "on"
       id division.
       program-id. prog.
       procedure division.
           >>IF FOO = "on"
           DISPLAY "FOO is on.".
           >>END-IF
           DISPLAY "gratuitous display.".
       goback.

