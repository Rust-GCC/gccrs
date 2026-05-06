       *> { dg-do run }
       *> { dg-options "-ffixed-form" }
       *> { dg-output-file "group2/FIXED_FORMAT_data_in_cols_73_and_beyond.out" }

       *> ISO-IEC2014 leaves the length of the Program Area in Fixed
       *> Format to the implementor.
       *> By convention it ends in position 72.
       *> IBM's COBOLs, Microfocus, GnuCOBOL follow that convention.
       IDENTIFICATION DIVISION.                                         VALID
       PROGRAM-ID. prog.
       PROCEDURE DIVISION.
       DISPLAY "OK"
       GOBACK.
       END PROGRAM prog.

