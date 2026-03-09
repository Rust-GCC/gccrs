       *> { dg-do run }
      *> TEST FUNCTION ACOS (Intrinsic)
       IDENTIFICATION DIVISION.
       PROGRAM-ID. prog.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       77 trig-val-1 PIC S9v999999.
       77 trig-val-2 PIC -9.999999.
       77 report-area PIC x(80).
       77 failure-count PIC 99 VALUE ZERO.
       77 failure-report PIC Z9 VALUE ZERO.
       PROCEDURE DIVISION.
       main-procedure.
           PERFORM run-tests.
           PERFORM report-failure-count.
       GOBACK.
           EXIT PROGRAM.

       run-tests.
           MOVE FUNCTION ACOS(0.707107) TO trig-val-1.
           MOVE trig-val-1 TO trig-val-2.
           IF trig-val-2 NOT EQUAL " 0.785397"
              STRING "FUNCTION ACOS(0.707107) FAILS. RETURNED "
                 trig-val-2 INTO report-area
              END-STRING
              PERFORM do-failure
              END-IF.
           MOVE FUNCTION ACOS(-0.707107) TO trig-val-1.
           MOVE trig-val-1 TO trig-val-2.
           IF trig-val-2 NOT EQUAL " 2.356194"
              STRING 'FUNCTION ACOS(-0.0707107 FAILS. '
                 'RETURNED ' trig-val-2 INTO report-area
              END-STRING
              PERFORM do-failure
              END-IF.
           MOVE FUNCTION ACOS(-1.000000) TO trig-val-1.
           MOVE trig-val-1 TO trig-val-2.
           IF trig-val-2 NOT EQUAL " 3.141592"
              STRING 'FUNCTION ACOS(-1.0000000 FAILS. '
                 'RETURNED ' trig-val-2 INTO report-area
              END-STRING
              PERFORM do-failure
              END-IF.
           MOVE FUNCTION ACOS(1.000000) TO trig-val-1.
           MOVE trig-val-1 TO trig-val-2.
           IF trig-val-2 NOT EQUAL " 0.000000"
              STRING 'FUNCTION ACOS(1.0000000 FAILS. '
                 'RETURNED ' trig-val-2 INTO report-area
              END-STRING
              PERFORM do-failure
              END-IF.
      *> ALLOWABLE RANGE for ACOS per ISO2014 (15.8.2):
      *>    The value of argument-1 shall be greater than or equal
      *>    to –1 and less than or equal to +1.
      *> No comment about how the IMPLEMENTER SHOULD/MUST handle out
      *> of range inputs.
      *>
      *> Because Exception Code processing is, by default, not turned on, the
      *> return value for a bad parameter comes back as zero
           MOVE FUNCTION ACOS(1.707107) TO trig-val-2.
           IF trig-val-2 NOT EQUAL " 0.000000"
              STRING 'FUNCTION ACOS(1.707107) FAILS. '
              'RETURNED ' trig-val-2 INTO report-area
              PERFORM do-failure
              END-IF.
           MOVE FUNCTION ACOS(-1.707107) TO trig-val-2.
           IF trig-val-2 NOT EQUAL " 0.000000"
              STRING 'FUNCTION ACOS(-1.707107) FAILS. '
              'RETURNED ' trig-val-2 INTO report-area
              PERFORM do-failure
              END-IF.

       do-failure.
           MOVE 1 TO RETURN-CODE,
           DISPLAY report-area,
           MOVE SPACE to report-area.
           ADD 1 TO failure-count.

       report-failure-count.
           IF failure-count IS GREATER THAN ZERO
           THEN
              MOVE failure-count TO failure-report
              DISPLAY "Total failures: " failure-report
           END-IF.

