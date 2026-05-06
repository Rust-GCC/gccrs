       *> { dg-do run }
       *> { dg-output-file "group2/INITIALIZE_complex_group__2_.out" }

       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01 G1.
         02 G2          OCCURS 2.
           03 X         PIC 9.
           03 Y         PIC X OCCURS 2.
           03 Z         PIC X.
       PROCEDURE        DIVISION.
           MOVE ALL 'Z' TO G1
           DISPLAY """"G1""""
           INITIALIZE G1
           DISPLAY """"G1""""
           IF G1 NOT = "0   0   "
              DISPLAY "That should have been ""0   0   """
              END-DISPLAY
           END-IF
           STOP RUN.

