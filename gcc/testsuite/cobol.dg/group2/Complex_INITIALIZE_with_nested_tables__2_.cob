       *> { dg-do run }
       *> { dg-output-file "group2/Complex_INITIALIZE_with_nested_tables__2_.out" }

        program-id. prog.
        data division.
        working-storage section.

        01 three-by-three2.
          05 label5 pic x(14) value "three-by-three".
          05 three-outer2 occurs 3 times.
            10 label10 pic x(12) value "leading".
            10 three-inner2 occurs 3 times.
              15 label15 pic x(12) value "three-inner".
              15 FNAME  PIC X(7) VALUE "James".
              15 FILLER PIC X(7) VALUE "Keen ".
              15 LNAME  PIC X(7) VALUE "Lowden".
            10 label10 pic x(12) value "middling".
            10 three-inner22 occurs 3 times.
              15 label15 pic x(12) value "three-inner".
              15 FNAME  PIC X(7) VALUE "James".
              15 FILLER PIC X(7) VALUE "Keen ".
              15 LNAME  PIC X(7) VALUE "Lowden".
            10 label10 pic x(12) value "trailing".

        procedure division.
        display "      Complex three-by-three table, with extra fields"
        display "50-1 " """" three-outer2(1) """"
        display "50-2 " """" three-outer2(2) """"
        display "50-3 " """" three-outer2(3) """"

        INITIALIZE three-by-three2.
        display "      After INITIALIZE, only the KEEN columns should be left"
        display "51-1 " """" three-outer2(1) """"
        display "51-2 " """" three-outer2(2) """"
        display "51-3 " """" three-outer2(3) """"
        INITIALIZE three-by-three2 WITH FILLER.
        display "      After INITIALIZE WITH FILLER, all should be blank"
        display "52-1 " """" three-outer2(1) """"
        display "52-2 " """" three-outer2(2) """"
        display "52-3 " """" three-outer2(3) """"
        INITIALIZE three-by-three2 ALL VALUE
        display "      After INITIALIZE ALL VALUE, all but the KEEN columns should be back"
        display "53-1 " """" three-outer2(1) """"
        display "53-2 " """" three-outer2(2) """"
        display "53-3 " """" three-outer2(3) """"
        INITIALIZE three-by-three2 WITH FILLER ALL VALUE
        display "      After INITIALIZE WITH FILLER ALL VALUE, should be the original"
        display "54-1 " """" three-outer2(1) """"
        display "54-2 " """" three-outer2(2) """"
        display "54-3 " """" three-outer2(3) """"

        move all 'Z' to three-by-three2
        move all 'A' to three-inner2(1 1)
        move all 'B' to three-inner2(1 2)
        move all 'C' to three-inner2(1 3)
        move all 'D' to three-inner2(2 1)
        move all 'E' to three-inner2(2 2)
        move all 'F' to three-inner2(2 3)
        move all 'G' to three-inner2(3 1)
        move all 'H' to three-inner2(3 2)
        move all 'I' to three-inner2(3 3)

        move all 'a' to three-inner22(1 1)
        move all 'b' to three-inner22(1 2)
        move all 'c' to three-inner22(1 3)
        move all 'd' to three-inner22(2 1)
        move all 'e' to three-inner22(2 2)
        move all 'f' to three-inner22(2 3)
        move all 'g' to three-inner22(3 1)
        move all 'h' to three-inner22(3 2)
        move all 'i' to three-inner22(3 3)

        display "      After setting FILLER fields to unique values"
        display "55-1 " """" three-outer2(1) """"
        display "55-2 " """" three-outer2(2) """"
        display "55-3 " """" three-outer2(3) """"

        INITIALIZE three-by-three2 ALL VALUE
        display "      After INITIALIZE ALL VALUE, the KEEN columns should have the unique values"
        display "56-1 " """" three-outer2(1) """"
        display "56-2 " """" three-outer2(2) """"
        display "56-3 " """" three-outer2(3) """"

        goback.

