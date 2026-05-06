       *> { dg-do run }
       *> { dg-output-file "group2/Long_Division.out" }
        identification      division.
        program-id.         prog.
        data                division.
        working-storage     section.
        01 a pic 9(37) display.
        01 b pic 9(37) display.
        01 c pic 9(37) display.
        procedure division.
        move 1000000000000000000000000000000000000 to b
        move  200000000000000000000000000000000000 to c
        divide b by c giving a
        display a.
        end program         prog.

