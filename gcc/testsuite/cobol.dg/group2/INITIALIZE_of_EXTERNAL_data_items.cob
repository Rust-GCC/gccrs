       *> { dg-do run }

       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01 EXT-VAR-01    PIC X(5) EXTERNAL.
       01 EXT-VAR-GRP   EXTERNAL.
         02  EXT-FIELD1 PIC 999.
         02  EXT-FIELD2 PIC x(4).
         02  EXT-FIELD3 PIC 9(6).
         02  EXT-FIELD4 PIC s9(5)v99.
       PROCEDURE        DIVISION.
           MOVE "MOVE"  TO EXT-VAR-01.
           MOVE 1       TO EXT-FIELD1.
           MOVE "X"     TO EXT-FIELD2.
           MOVE 123     TO EXT-FIELD3.
           MOVE -2.1    TO EXT-FIELD4.
           INITIALIZE EXT-VAR-01.
           INITIALIZE EXT-VAR-GRP.
           IF EXT-VAR-01 NOT = SPACES
              DISPLAY "EXT-VAR-01 " EXT-VAR-01
              END-DISPLAY
           END-IF.
           IF EXT-FIELD1 NOT = ZERO
              DISPLAY "EXT-FIELD1 " EXT-FIELD1
              END-DISPLAY
           END-IF.
           IF EXT-FIELD2 NOT = SPACES
              DISPLAY "EXT-FIELD2 " EXT-FIELD2
              END-DISPLAY
           END-IF.
           IF EXT-FIELD3 NOT = ZERO
              DISPLAY "EXT-FIELD3 " EXT-FIELD3
              END-DISPLAY
           END-IF.
           IF EXT-FIELD4 NOT = ZERO
              DISPLAY "EXT-FIELD4 " EXT-FIELD4
              END-DISPLAY
           END-IF.
           STOP RUN.

