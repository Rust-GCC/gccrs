       *> { dg-do run }
       *> { dg-xfail-run-if "" { *-*-* }  }

       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01 I             PIC 99 COMP.
       PROCEDURE        DIVISION.
           INITIALIZE RETURN-CODE.
           MOVE ZERO TO RETURN-CODE.
           MOVE 1 TO RETURN-CODE.
           MOVE RETURN-CODE TO I.
           IF I NOT = 1
              DISPLAY I NO ADVANCING
              END-DISPLAY
           END-IF.
           STOP RUN.

