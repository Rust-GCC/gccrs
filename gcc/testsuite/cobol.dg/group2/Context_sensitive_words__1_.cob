       *> { dg-do run }
       *> { dg-output-file "group2/Context_sensitive_words__1_.out" }

       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01  BYTE-LENGTH  PIC 9.
       01  X            CONSTANT AS BYTE-LENGTH OF BYTE-LENGTH.
       01  stride       binary-short.
       PROCEDURE        DIVISION.
           move function byte-length("A") to stride
           MOVE X TO BYTE-LENGTH.
           compute byte-length = x / stride
           DISPLAY BYTE-LENGTH NO ADVANCING
           END-DISPLAY.
           STOP RUN.

