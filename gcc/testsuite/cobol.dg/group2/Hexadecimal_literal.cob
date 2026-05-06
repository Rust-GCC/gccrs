       *> { dg-do run }
       *> { dg-output-file "group2/Hexadecimal_literal.out" }
       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01 A PIC X VALUE "0".
       PROCEDURE        DIVISION.
           *> Detect EBCDIC vs ASCII vs UTF
           evaluate A
                when X"F0" display X"F1F2F3"
                when X"30" DISPLAY X"313233"
                when X"3000" DISPLAY X"310032003300"
                when other display "BaCK to the drawing board"
           goback.

