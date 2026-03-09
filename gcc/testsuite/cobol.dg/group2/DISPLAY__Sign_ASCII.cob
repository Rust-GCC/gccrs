       *> { dg-do run }
       *> { dg-output-file "group2/DISPLAY__Sign_ASCII.out" }
       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      prog.
       DATA             DIVISION.
       WORKING-STORAGE  SECTION.
       01 G.
         02 X           PIC X(5).
         02 X-9         REDEFINES X PIC 9(4).
         02 X-S9        REDEFINES X PIC S9(4).
         02 X-S9-L      REDEFINES X PIC S9(4) LEADING.
         02 X-S9-LS     REDEFINES X PIC S9(4) LEADING SEPARATE.
         02 X-S9-T      REDEFINES X PIC S9(4) TRAILING.
         02 X-S9-TS     REDEFINES X PIC S9(4) TRAILING SEPARATE.
       PROCEDURE        DIVISION.
           MOVE ZERO TO X MOVE  1234 TO X-9     DISPLAY X
           MOVE ZERO TO X MOVE  1234 TO X-S9    DISPLAY X
           MOVE ZERO TO X MOVE -1234 TO X-S9
           *> Let's be ecumenical with regard to ASCII and EBCDIC:
           if X equals "123M0" or "123t0" DISPLAY "-1234" 
                                     else DISPLAY X " Not Good"
                                     end-if
           MOVE ZERO TO X MOVE  1234 TO X-S9-L  DISPLAY X
           MOVE ZERO TO X MOVE -1234 TO X-S9-L
           if X equals "J2340" or "q2340" DISPLAY "-1234" 
                                     else DISPLAY X " Not Good"
                                     end-if
           MOVE ZERO TO X MOVE  1234 TO X-S9-LS DISPLAY X
           MOVE ZERO TO X MOVE -1234 TO X-S9-LS DISPLAY X
           MOVE ZERO TO X MOVE  1234 TO X-S9-T  DISPLAY X
           MOVE ZERO TO X MOVE -1234 TO X-S9-T
           if X equals "123M0" or "123t0" DISPLAY "-1234" 
                                     else DISPLAY X " Not Good"
                                     end-if
           MOVE ZERO TO X MOVE  1234 TO X-S9-TS DISPLAY X
           MOVE ZERO TO X MOVE -1234 TO X-S9-TS DISPLAY X
           goback.
           end program  prog.

