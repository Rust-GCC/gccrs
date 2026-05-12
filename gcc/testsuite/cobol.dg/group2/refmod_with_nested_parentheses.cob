       *> { dg-do run }
       *> { dg-options "-dialect ibm" }
       *> { dg-output-file "group2/refmod_with_nested_parentheses.out" }
        IDENTIFICATION DIVISION.
        PROGRAM-ID. refmod_nested_paren_expr.
        DATA DIVISION.
        WORKING-STORAGE SECTION.
        01  TXT                        PIC X(10) VALUE 'ABCDEFGHIJ'.
        01  N                          PIC 9      VALUE 2.
        01  OUT-CH                     PIC X.
        01  stride                     PIC 9.
        PROCEDURE DIVISION.
      *> LENGTH OF is bytes, so we need stride to handle UTF-16
            move length of out-ch to stride
            display TXT(((LENGTH OF TXT / (2*stride)) - (N / 2)):1)
            MOVE TXT(((LENGTH OF TXT / (2*stride)) - (N / 2)):1) 
                                                              TO OUT-CH
            display OUT-CH
            GOBACK.

