       *> { dg-do run }

       ID DIVISION.
       PROGRAM-ID. prog.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01 var1 PIC X100.
       PROCEDURE DIVISION.
          ACCEPT var1 FROM COMMAND-LINE(1).
          DISPLAY var1.
          GOBACK.

