       *> { dg-do run }

       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       PROCEDURE        DIVISION.
           MOVE 1 TO RETURN-CODE.
           IF RETURN-CODE NOT = 1
              DISPLAY RETURN-CODE NO ADVANCING
              END-DISPLAY
           END-IF.
           CALL "mod1"
           END-CALL.
           IF RETURN-CODE NOT = 2
              DISPLAY RETURN-CODE NO ADVANCING
              END-DISPLAY
           END-IF.
           MOVE ZERO TO RETURN-CODE.
           STOP RUN.
       PROGRAM-ID.      mod1.
       PROCEDURE        DIVISION.
           IF RETURN-CODE NOT = 1
              DISPLAY RETURN-CODE NO ADVANCING
              END-DISPLAY
           END-IF.
           MOVE 2 TO RETURN-CODE.
           EXIT PROGRAM.
       END PROGRAM mod1.
       END PROGRAM prog.

