       *> { dg-do run }

       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01 G1.
         02 X           PIC 99.
         02 FILLER      PIC X.
         02 Z           PIC 99.
       01 MY-FILLER.
         02 FILLER      PIC 9(6) VALUE 12345.
       PROCEDURE        DIVISION.
           MOVE ALL   'A' TO G1.
           INITIALIZE G1.
           IF G1 NOT = "00A00"
              DISPLAY "G1 (INIT): " G1
              END-DISPLAY
           END-IF.
           MOVE ALL   'A' TO G1.
           INITIALIZE G1  WITH FILLER.
           IF G1 NOT = "00 00"
              DISPLAY "G1 (INIT FILLER):"   G1
              END-DISPLAY
           END-IF.

           INITIALIZE MY-FILLER
           IF MY-FILLER NOT = "012345"
              DISPLAY "MY-FILLER (INIT): " MY-FILLER
              END-DISPLAY
           END-IF

           INITIALIZE MY-FILLER WITH FILLER
           IF MY-FILLER NOT = "000000"
              DISPLAY "MY-FILLER (INIT FILLER): " MY-FILLER
              END-DISPLAY
           END-IF

           INITIALIZE MY-FILLER ALL TO VALUE
           IF MY-FILLER NOT = "000000"
              DISPLAY "MY-FILLER (INIT TO VAL): " MY-FILLER
              END-DISPLAY
           END-IF

           INITIALIZE MY-FILLER WITH FILLER ALL TO VALUE
           IF MY-FILLER NOT = "012345"
              DISPLAY "MY-FILLER (INIT FILLER TO VAL): " MY-FILLER
              END-DISPLAY
           END-IF

           INITIALIZE MY-FILLER (2:3)
           IF MY-FILLER NOT = "0   45"
              DISPLAY "MY-FILLER (REF-MOD): " MY-FILLER
              END-DISPLAY
           END-IF

           STOP RUN.

