       *> { dg-do run }
       *> { dg-options "-dialect ibm" }

       IDENTIFICATION DIVISION.
       PROGRAM-ID. FLTDISP.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01  F1 COMP-1 VALUE 1.5.
       01  F2 COMP-1 VALUE 0.1.
       01  F3 COMP-1 VALUE -2.75.
       01  F4 COMP-1 VALUE 1500.
       01  D1 COMP-2 VALUE 2.71828.
       PROCEDURE DIVISION.
           DISPLAY F1
           DISPLAY F2
           DISPLAY F3
           DISPLAY F4
           DISPLAY D1
           STOP RUN.

