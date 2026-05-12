       *> { dg-do run }
       *> { dg-output-file "group2/REDEFINES__chained.out" }
        IDENTIFICATION   DIVISION.
        PROGRAM-ID.      chained_REDEFINES.
        DATA             DIVISION.
        WORKING-STORAGE  SECTION.
        01 REC.
        10 ZIP-9                    PIC 9(9).
        10 ZIP-RED REDEFINES ZIP-9.
            12 ZIP-5                 PIC 9(5).
            12 ZIP-PLUS-4            PIC 9(4).
        10 POSTAL-CORRECT-IND REDEFINES ZIP-9.
            12 FILLER                PIC X(8).
            12 POST-CORRT-IND        PIC X(1).
        PROCEDURE        DIVISION.
            MOVE 123456789 TO ZIP-9
            DISPLAY """" ZIP-9 """"
            IF ZIP-5 NOT = 12345
            DISPLAY "ZIP-5 wrong: " ZIP-5
            END-DISPLAY
            END-IF
            DISPLAY """" ZIP-PLUS-4 """"
            IF ZIP-PLUS-4 NOT = 6789
            DISPLAY "ZIP-PLUS-4 wrong: " ZIP-PLUS-4
            END-DISPLAY
            END-IF
            MOVE "X" TO POST-CORRT-IND
            DISPLAY """" ZIP-9 """"
            DISPLAY """" POSTAL-CORRECT-IND  """"
            DISPLAY """" POST-CORRT-IND """"
            IF POST-CORRT-IND NOT = "X"
            DISPLAY "POST-CORRT-IND wrong: " POST-CORRT-IND
            END-DISPLAY
            END-IF
            STOP RUN.

