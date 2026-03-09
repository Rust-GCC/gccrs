       *> { dg-do run }
       *> { dg-options "-fdefaultbyte 64" }
       *> { dg-output-file "group2/INITIALIZE_with_-defaultbyte__ASCII_.out" }

        IDENTIFICATION   DIVISION.
        PROGRAM-ID.      proga.
        PROCEDURE        DIVISION.
        call "prog"
        call "prog"
        goback.
        end program proga.
        IDENTIFICATION   DIVISION.
        PROGRAM-ID.      prog.
        DATA             DIVISION.
        WORKING-STORAGE  SECTION.
        01  MY-FLD       PIC X(6) VALUE "ABCDEF".
        01  MY-OTHER-FLD PIC 9(4) VALUE ZERO.
        01  FILLER.
           02 PHONE-NUMBER.
              03 NAME      PIC A(30).
              03 AREA-CODE PIC 999.
              03 DASH      PIC X VALUE'-'.
              03 PREFIX    PIC 999.
              03 DASH      PIC X VALUE'-'.
              03 LOCAL     PIC 999.
        77 WHO-AM-I PIC X(12).
        PROCEDURE        DIVISION.
        ASTART SECTION.
        A01.
        DISPLAY MY-FLD.
        DISPLAY MY-OTHER-FLD.
        DISPLAY PHONE-NUMBER.
        DISPLAY WHO-AM-I.
        move quote to phone-number
        display """" phone-number """"
        goback.
        end program prog.

