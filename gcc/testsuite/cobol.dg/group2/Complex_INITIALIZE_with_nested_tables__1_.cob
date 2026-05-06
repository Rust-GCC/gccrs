       *> { dg-do run }
       *> { dg-output-file "group2/Complex_INITIALIZE_with_nested_tables__1_.out" }

        program-id. prog.
        data division.
        working-storage section.

        01 foo.
          05 FNAME  PIC X(7) VALUE "James".
          05 FILLER PIC X(7) VALUE "Keen ".
          05 LNAME  PIC X(7) VALUE "Lowden".

        01 filler  PIC 9999 BINARY value zero.

        01 foo3.
          02 three-lines occurs 3 times.
            05 FNAME  PIC X(7) VALUE "James".
            05 FILLER PIC X(7) VALUE "Keen ".
            05 LNAME  PIC X(7) VALUE "Lowden".

        01 filler  PIC 9999 BINARY value zero.

        01 four-by-four.
          05 four-outer occurs 4 times.
            10 four-inner occurs 4 times.
              15 FNAME  PIC X(7) VALUE "James".
              15 FILLER PIC X(7) VALUE "Keen ".
              15 LNAME  PIC X(7) VALUE "Lowden".

        01 filler  PIC 9999 BINARY value zero.

        01 four-by-four2.
          05 label5 pic x(12) value "four-by-four".
          05 four-outer2 occurs 4 times.
            10 label10 pic x(12) value "four-outer".
            10 four-inner2 occurs 4 times.
              15 label15 pic x(12) value "four-inner".
              15 FNAME  PIC X(7) VALUE "James".
              15 FILLER PIC X(7) VALUE "Keen ".
              15 LNAME  PIC X(7) VALUE "Lowden".

        procedure division.
        display "   Simple data structure"
        display "1 " """" foo """".
        INITIALIZE foo.
        display "2 " """" foo """".
        INITIALIZE foo WITH FILLER.
        display "3 " """" foo """".
        INITIALIZE foo ALL VALUE
        display "4 " """" foo """".
        INITIALIZE foo WITH FILLER ALL VALUE
        display "5 " """" foo """".

        display "    Simple table"
        display "31 " """" foo3 """".
        initialize foo3
        display "32 " """" foo3 """".
        INITIALIZE foo3 WITH FILLER.
        display "33 " """" foo3 """".
        INITIALIZE foo3 ALL VALUE
        display "34 " """" foo3 """".
        INITIALIZE foo3 WITH FILLER ALL VALUE
        display "35 " """" foo3 """".
        
        move all "A" to three-lines(1)
        move all "B" to three-lines(2)
        move all "C" to three-lines(3)
        display "36 " """" foo3 """".
        INITIALIZE foo3 ALL VALUE
        display "37 " """" foo3 """".

        display "      Simple four-by-four table"
        display "40-1 " """" four-outer(1) """"
        display "40-2 " """" four-outer(2) """"
        display "40-3 " """" four-outer(3) """"
        display "40-4 " """" four-outer(4) """"

        move all 'A' to four-inner(1 1)
        move all 'B' to four-inner(1 2)
        move all 'C' to four-inner(1 3)
        move all 'D' to four-inner(1 4)
        move all 'E' to four-inner(2 1)
        move all 'F' to four-inner(2 2)
        move all 'G' to four-inner(2 3)
        move all 'H' to four-inner(2 4)
        move all 'I' to four-inner(3 1)
        move all 'J' to four-inner(3 2)
        move all 'K' to four-inner(3 3)
        move all 'L' to four-inner(3 4)
        move all 'M' to four-inner(4 1)
        move all 'N' to four-inner(4 2)
        move all 'O' to four-inner(4 3)
        move all 'P' to four-inner(4 4)

        display "41-1 " """" four-outer(1) """"
        display "41-2 " """" four-outer(2) """"
        display "41-3 " """" four-outer(3) """"
        display "41-4 " """" four-outer(4) """"

        INITIALIZE four-by-four ALL VALUE

        display "42-1 " """" four-outer(1) """"
        display "42-2 " """" four-outer(2) """"
        display "42-3 " """" four-outer(3) """"
        display "42-4 " """" four-outer(4) """"

        display "      Complex four-by-four table, with extra fields"
        display "50-1 " """" four-outer2(1) """"
        display "50-2 " """" four-outer2(2) """"
        display "50-3 " """" four-outer2(3) """"
        display "50-4 " """" four-outer2(4) """"

        INITIALIZE four-by-four2.
        display "      After INITIALIZE, only the KEEN columns should be left"
        display "51-1 " """" four-outer2(1) """"
        display "51-2 " """" four-outer2(2) """"
        display "51-3 " """" four-outer2(3) """"
        display "51-4 " """" four-outer2(4) """"
        INITIALIZE four-by-four2 WITH FILLER.
        display "      After INITIALIZE WITH FILLER, all should be blank"
        display "52-1 " """" four-outer2(1) """"
        display "52-2 " """" four-outer2(2) """"
        display "52-3 " """" four-outer2(3) """"
        display "52-4 " """" four-outer2(4) """"
        INITIALIZE four-by-four2 ALL VALUE
        display "      After INITIALIZE ALL VALUE, all but the KEEN columns should be back"
        display "53-1 " """" four-outer2(1) """"
        display "53-2 " """" four-outer2(2) """"
        display "53-3 " """" four-outer2(3) """"
        display "53-4 " """" four-outer2(4) """"
        INITIALIZE four-by-four2 WITH FILLER ALL VALUE
        display "      After INITIALIZE WITH FILLER ALL VALUE, should be the original"
        display "54-1 " """" four-outer2(1) """"
        display "54-2 " """" four-outer2(2) """"
        display "54-3 " """" four-outer2(3) """"
        display "54-4 " """" four-outer2(4) """"

        move all 'A' to four-inner2(1 1)
        move all 'B' to four-inner2(1 2)
        move all 'C' to four-inner2(1 3)
        move all 'D' to four-inner2(1 4)
        move all 'E' to four-inner2(2 1)
        move all 'F' to four-inner2(2 2)
        move all 'G' to four-inner2(2 3)
        move all 'H' to four-inner2(2 4)
        move all 'I' to four-inner2(3 1)
        move all 'J' to four-inner2(3 2)
        move all 'K' to four-inner2(3 3)
        move all 'L' to four-inner2(3 4)
        move all 'M' to four-inner2(4 1)
        move all 'N' to four-inner2(4 2)
        move all 'O' to four-inner2(4 3)
        move all 'P' to four-inner2(4 4)

        display "      After setting FILLER fields to unique values"
        display "55-1 " """" four-outer2(1) """"
        display "55-2 " """" four-outer2(2) """"
        display "55-3 " """" four-outer2(3) """"
        display "55-4 " """" four-outer2(4) """"

        INITIALIZE four-by-four2 ALL VALUE
        display "      After INITIALIZE ALL VALUE, the KEEN columns should have the unique values"
        display "56-1 " """" four-outer2(1) """"
        display "56-2 " """" four-outer2(2) """"
        display "56-3 " """" four-outer2(3) """"
        display "56-4 " """" four-outer2(4) """"

        goback.

