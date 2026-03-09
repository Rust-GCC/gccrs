       *> { dg-do run }
       *> { dg-options "-dialect ibm" }
       *> { dg-output-file "group2/CDF__1__IF____text_.out" }

       >>DEFINE FOO AS "on"
       id division.
       program-id. prog.
       Data Division.
       Working-Storage Section.
         77 X PIC 9 value 1.
       procedure division.
           >>IF FOO = "on"
           DISPLAY "                 FOO is on.".
           >>END-IF
           DISPLAY "Should have seen FOO is on.".
           >>IF FOO = "off"
           DISPLAY "                 FOO is off.".
           >>END-IF
           DISPLAY "Shouldn't see    FOO is off.".
         a-paragraph.
           EJECT
         a-paragraph.
           add 1 to X.
           EJECT
         a-paragraph.
           EJECT
         b-paragraph.
           goback.

