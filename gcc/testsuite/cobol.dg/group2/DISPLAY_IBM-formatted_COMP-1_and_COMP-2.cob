       *> { dg-do run }
       *> { dg-options "-dialect ibm" }
       *> { dg-output-file "group2/DISPLAY_IBM-formatted_COMP-1_and_COMP-2.out" }
        identification              division.
        program-id.                 prog.
        data                        division.
        working-storage             section.
        01  f1 comp-1 value 1.5.
        01  f2 comp-1 value 0.1.
        01  f3 comp-1 value -2.75.
        01  f4 comp-1 value 1500.
        01  f5 comp-1 value 0.001500.
        01  d1 comp-2 value 2.71828.
        01  d2 comp-2 value -0.0000271828.
        procedure                   division.
            display f1
            display f2
            display f3
            display f4
            display f5
            display d1
            display d2
            goback.
        end program                 prog.

