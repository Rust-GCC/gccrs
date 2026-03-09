       *> { dg-do run }
       *> { dg-options "-ffixed-form" }

       IDENTIFICATION DIVISION.
       PROGRAM-ID. prog.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       77 var PIC 99 VALUE ZERO.
       PROCEDURE DIVISION.
       COMPUTE VAR = 5
      * 3
       + 6.
       IF var NOT = 11
          MOVE 1 to RETURN-CODE
          DISPLAY var.
       GOBACK.
       END PROGRAM prog.

