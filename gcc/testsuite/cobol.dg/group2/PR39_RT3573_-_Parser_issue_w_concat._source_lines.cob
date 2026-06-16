      *> { dg-do compile }

            >>SOURCE FORMAT IS FREE
        IDENTIFICATION DIVISION.  PROGRAM-ID. issue4.  DATA DIVISION.
        WORKING-STORAGE SECTION.
        1 hello-string PIC X(12) VALUE "Hello string".
        PROCEDURE DIVISION.
        DISPLAY hello-string.
        EXIT PROGRAM.

