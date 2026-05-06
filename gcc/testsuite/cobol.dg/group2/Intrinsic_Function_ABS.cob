       *> { dg-do run }
       IDENTIFICATION DIVISION.
       PROGRAM-ID. prog.
       PROCEDURE DIVISION.
           IF FUNCTION ABS(123.4) NOT EQUAL TO 123.4
                   MOVE 1 TO RETURN-CODE
                   DISPLAY "FUNCTION ABS(123.4) FAILS."
                   END-IF.
           IF FUNCTION ABS(-123.4) NOT EQUAL TO 123.4
                   MOVE 1 TO RETURN-CODE
                   DISPLAY "FUNCTION ABS(-123.4) FAILS "
                   END-IF.
           IF FUNCTION ABS(-000.0) NOT EQUAL TO ZERO
                   MOVE 1 TO RETURN-CODE
                   DISPLAY "FUNCTION ABS(-000.0) FAILS."
                   END-IF.
           IF FUNCTION ABS(000.0) NOT EQUAL TO ZERO
                   MOVE 1 TO RETURN-CODE
                   DISPLAY "FUNCTION ABS(-000.0) FAILS."
                   END-IF.

