       *> { dg-do run }

       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01 G1.
         02 G2          OCCURS 5.
           03 X         PIC Z9.
       PROCEDURE        DIVISION.
           INITIALIZE G1
           MOVE 5  TO X(1)
           MOVE 99 TO X(3)
           IF G1 NOT = " 5 099 0 0"
              DISPLAY 'MOVE "' G1 '"'
              END-DISPLAY
           END-IF
           INITIALIZE G1
           IF G1 NOT = " 0 0 0 0 0"
              DISPLAY 'INIT "' G1 '"'
              END-DISPLAY
           END-IF
           STOP RUN.

