       *> { dg-do run }
      *> TEST FUNCTION ANNUITY (Intrinsic)
      *> INADEQUATE sample of tests: TODO FIXME
       IDENTIFICATION DIVISION.
       PROGRAM-ID. prog.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       77 trig-val-1 PIC S9v999999.
       PROCEDURE DIVISION.
           MOVE FUNCTION ANNUITY(0.07, 12) TO trig-val-1.
           IF trig-val-1 NOT EQUAL +0.125901
                   MOVE 1 TO RETURN-CODE
                   DISPLAY 'FUNCTION ANNUITY(0.07, 12) FAILS.'
                   DISPLAY 'RETURNED ' trig-val-1 ', not 0.125901'
                   END-IF.

