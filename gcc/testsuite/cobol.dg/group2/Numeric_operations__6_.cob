       *> { dg-do run }
       *> { dg-output-file "group2/Numeric_operations__6_.out" }

        IDENTIFICATION DIVISION.
        PROGRAM-ID. prog.

        DATA DIVISION.
        WORKING-STORAGE SECTION.

        01 P-FIELD1 PIC 99PPP.
        01 p1 redefines p-field1 pic x(2).
        01 P-FIELD2 PIC PPP99.
        01 p2 redefines p-field2 pic x(2).

        PROCEDURE DIVISION.

        MOVE 5000 TO P-FIELD1.
        ADD 5 TO P-FIELD1 END-ADD
        IF P-FIELD1 NOT = 5000
            DISPLAY "Error: Add 5 to PIC 99PPP."
            END-DISPLAY
        END-IF
        display p1

        ADD 5000 TO P-FIELD1 END-ADD
        IF P-FIELD1 NOT = 10000
            DISPLAY "Error: Add 5000 to PIC 99PPP."
            END-DISPLAY
        END-IF
        display p1

        MOVE 0.00055 TO P-FIELD2.
        ADD 0.00033 TO P-FIELD2 END-ADD
        IF P-FIELD2 NOT = 0.00088
            DISPLAY "Error: Add 0.00033 to PIC PPP99."
            END-DISPLAY
        END-IF
        display p2

        MOVE 0.00055 TO P-FIELD2.
        ADD 0.00300 TO P-FIELD2 END-ADD
        IF P-FIELD2 NOT = 0.00055
            DISPLAY "Error: Add 0.00300 to PIC PPP99."
            END-DISPLAY
        END-IF
        display p2

        STOP RUN.

