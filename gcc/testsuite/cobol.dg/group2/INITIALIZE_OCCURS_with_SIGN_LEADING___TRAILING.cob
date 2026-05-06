       *> { dg-do run }
       *> { dg-output-file "group2/INITIALIZE_OCCURS_with_SIGN_LEADING___TRAILING.out" }
       >>SOURCE FIXED
       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01 G1.
         02 FILLER      OCCURS 2.
           03 X         PIC S9 SIGN LEADING  SEPARATE.
         02 FILLER      OCCURS 2.
           03 Y         PIC S9 SIGN TRAILING SEPARATE.
      *> definition taken from NC1184.2
       01  MINUS-NAMES SIGN IS TRAILING SEPARATE CHARACTER.
         02 MINUS-NAMES-1.
           03  MINUS-NAME1 PIC  S9(18) VALUE -999999999999999999.
           03  EVEN-NAME1  PIC  S9(18) VALUE +1.
           03  PLUS-NAME1  PIC  S9(18) VALUE +999999999999999999.
         02 MINUS-NAMES-2.
           03  MINUS-NAME3 PIC SV9(18) VALUE -.999999999999999999.
           03  EVEN-NAME2  PIC SV9(18) VALUE +.1.
           03  PLUS-NAME3  PIC SV9(18) VALUE +.999999999999999999.
       PROCEDURE        DIVISION.
           INITIALIZE G1
           MOVE 5    TO X(1), PLUS-NAME1
           MOVE -9   TO Y(2), MINUS-NAME1
           IF G1 NOT = "+5+00+9-"
              DISPLAY 'MOVE G "' G1 '"'
              END-DISPLAY
           END-IF
      **   The following line doesn't work causing test failure.
           MOVE .123 TO PLUS-NAME3
           IF MINUS-NAMES-1 NOT =
           "000000000000000009-000000000000000001+000000000000000005+"
           OR MINUS-NAMES-2 NOT =
           "999999999999999999-100000000000000000+123000000000000000+"
              DISPLAY 'MOVE MN1 "' MINUS-NAMES-1 '"'
              END-DISPLAY
              DISPLAY 'MOVE MN2 "' MINUS-NAMES-2 '"'
              END-DISPLAY
           END-IF
           INITIALIZE G1, MINUS-NAMES
           IF G1 NOT = "+0+00+0+"
              DISPLAY 'INIT G1 "' G1 '"'
              END-DISPLAY
           END-IF
           IF MINUS-NAMES-1 NOT =
           "000000000000000000+000000000000000000+000000000000000000+"
           OR MINUS-NAMES-2 NOT =
           "000000000000000000+000000000000000000+000000000000000000+"
              DISPLAY 'MOVE MN1 "' MINUS-NAMES-1 '"'
              END-DISPLAY
              DISPLAY 'MOVE MN2 "' MINUS-NAMES-2 '"'
              END-DISPLAY
           END-IF
           MOVE .123 TO PLUS-NAME3
           MOVE -.456 TO MINUS-NAME3
           DISPLAY PLUS-NAME3  END-DISPLAY
           DISPLAY MINUS-NAME3 END-DISPLAY
           STOP RUN.

