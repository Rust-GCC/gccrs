       *> { dg-do run }

       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01 G1.
         02 X           PIC X.
         02 Y           REDEFINES X PIC 9.
         02 Z           PIC 9.
       PROCEDURE        DIVISION.
           INITIALIZE G1.
           IF G1 NOT = " 0"
              DISPLAY G1
              END-DISPLAY
           END-IF.
           STOP RUN.

