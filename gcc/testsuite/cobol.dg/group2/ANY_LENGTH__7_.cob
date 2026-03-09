       *> { dg-do run }
       *> { dg-options "-Wno-any-length" }
       *> { dg-output-file "group2/ANY_LENGTH__7_.out" }
        IDENTIFICATION   DIVISION.
        PROGRAM-ID.      callee.
        DATA             DIVISION.
        WORKING-STORAGE  SECTION.
        01 P2            PIC 99.
        LINKAGE          SECTION.
        01 P1            PIC X ANY LENGTH.
        PROCEDURE        DIVISION USING P1.
            MOVE FUNCTION LENGTH (P1) TO P2.
            DISPLAY "The incoming ANY LENGTH is " P2
            DISPLAY "The incoming ANY LENGTH variable is " """" P1 """"
            EXIT PROGRAM.
        END PROGRAM callee.

