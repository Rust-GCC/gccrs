       *> { dg-do run }

       ID DIVISION.
       PROGRAM-ID. prog.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01 var1 PIC 99.
       PROCEDURE DIVISION.
          ADD 1 2 TO 4 GIVING var1.
          IF var1 NOT EQUAL 7
          THEN
             DISPLAY "Wrong answer, expected 7, got " var1 "."
          END-IF.
          GOBACK.

