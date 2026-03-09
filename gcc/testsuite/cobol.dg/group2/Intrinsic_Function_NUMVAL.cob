       *> { dg-do run }
       *> { dg-output-file "group2/Intrinsic_Function_NUMVAL.out" }
       IDENTIFICATiON DIVISION.
       PROGRAM-ID. prog.
       PROCEDURE DIVISION.
           DISPLAY FUNCTION NUMVAL(" 123.4 ").
           IF FUNCTION NUMVAL("  123.4     ") NOT EQUAL 123.4
              DISPLAY 'NUMVAL("  123.4  ") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" + 123.4     ") NOT EQUAL 123.4
              DISPLAY 'NUMVAL(" + 123.4  ") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL("+123.4") NOT EQUAL 123.4
              DISPLAY 'NUMVAL("+123.4") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL("  123.4 + ") NOT EQUAL 123.4
              DISPLAY 'NUMVAL(" 123.4 + ") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" - 123.4 ") NOT EQUAL -123.4
              DISPLAY 'NUMVAL(" - 123.4 ") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" - 123.4 ") NOT EQUAL -123.4
              DISPLAY 'NUMVAL(" - 123.4 ") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" 123.4 - ") NOT EQUAL -123.4
              DISPLAY 'NUMVAL(" 123.4 - ") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" 123.4 CR ") NOT EQUAL -123.4
              DISPLAY 'NUMVAL(" 123.4 CR ") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL("123.4cR") NOT EQUAL -123.4
              DISPLAY 'NUMVAL("123.4cR") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" 123.4 Cr ") NOT EQUAL -123.4
              DISPLAY 'NUMVAL(" 123.4 Cr ") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" 123.4 cr ") NOT EQUAL -123.4
              DISPLAY 'NUMVAL(" 123.4 cr ") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" 123.4 DB ") NOT EQUAL -123.4
              DISPLAY 'NUMVAL(" 123.4 DB") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" 123.4 dB ") NOT EQUAL -123.4
              DISPLAY 'NUMVAL(" 123.4 dB") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" 123.4 Db ") NOT EQUAL -123.4
              DISPLAY 'NUMVAL(" 123.4 Db") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" 123.4 db ") NOT EQUAL -123.4
              DISPLAY 'NUMVAL(" 123.4 db") FAILS'
              MOVE 1 TO RETURN-CODE
              END-IF.
      *> ISO2014 inputs to NUMVAL() "SHALL BE" and goes on to define
      *> a grammar of allowable strings. It is silent on correct
      *> behavior when the string presened does not conform to the
      *> grammer.
      *>
      *> As IMPLEMENTER we get to decide how to handle non-coforming
      *> input values. These tests use a return value of ZERO as a
      *> WORKING ASSUMPTION (TODISCUSS)
           IF FUNCTION NUMVAL(" ") NOT EQUAL ZERO
              DISPLAY 'NUMVAL(" ") FAILS'
                   DISPLAY 'NUMVAL(" ") FAILS WITH '
                       FUNCTION NUMVAL(" ")
              MOVE 1 TO RETURN-CODE
              END-IF.
           IF FUNCTION NUMVAL(" F ") NOT EQUAL ZERO
                   DISPLAY 'NUMVAL(" F ") FAILS WITH '
                       FUNCTION NUMVAL(" F ")
              MOVE 1 TO RETURN-CODE
              END-IF.

