       *> { dg-do run }
       *> { dg-output-file "group2/UNSTRING_with_refmods.out" }
        identification      division.
        program-id          prog.
        data                division.
        working-storage     section.
        01 foo pic x(10)  value "A12345678A".
        01 bar pic X(11) value all ".".
        procedure           division.
            unstring foo(2:) into bar(7:4) bar(2:4)
            display bar
            goback.

