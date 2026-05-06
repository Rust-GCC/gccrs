       *> { dg-do run }

       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01  MY-FLD       PIC X(6) VALUE "ABCDEF".
       01.
       02  MY-OTHER-FLD PIC 9(4) VALUE ZERO.
       02  AS-STRING REDEFINES MY-OTHER-FLD PIC X(4).
       PROCEDURE        DIVISION.
       ASTART SECTION.
       A01.
           INITIALIZE MY-FLD (1:2).
           IF MY-FLD NOT = "  CDEF"
              DISPLAY "MY-FLD: " MY-FLD
              END-DISPLAY
           END-IF
      *>  note: INITIALIZE with refmod => handle field as alphanumeric
           INITIALIZE MY-OTHER-FLD (2:2)
           MOVE "0  0" TO MY-FLD
           IF AS-STRING NOT = MY-FLD (1:4)
               DISPLAY "MY-OTHER-FLD: " MY-OTHER-FLD
               END-DISPLAY
           END-IF
           STOP RUN.

