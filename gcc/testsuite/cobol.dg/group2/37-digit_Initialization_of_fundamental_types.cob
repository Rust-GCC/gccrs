       *> { dg-do run }
       *> { dg-output-file "group2/37-digit_Initialization_of_fundamental_types.out" }
        identification      division.
        program-id.         prog.
        data                division.
        working-storage     section.
        01 foo1 pic 9(30)v9(7)        value 123456789012345678901234567890.1234567.
        01 foo2 pic 9(30)v9(7) comp-3 value 123456789012345678901234567890.1234567.
        01 foo3 pic 9(30).9(7)        value 123456789012345678901234567890.1234567.
        01 foo4 pic 9(30)v9(7) binary value 123456789012345678901234567890.1234567.
        01 foo5 pic 9(30)v9(7) comp-5 value 123456789012345678901234567890.1234567.
        01 foo6 pic 9(30)v9(7) binary-double 
                                      value 123456789012345678901234567890.1234567.
        procedure           division.
        display foo1
        display foo2
        display foo3
        display foo4
        display foo5
        display foo6
        move 111111111122222222223333333333.7654321 to foo1 foo2 foo3 foo4 foo5 foo6
        display foo1
        display foo2
        display foo3
        display foo4
        display foo5
        display foo6
        goback.
        end program         prog.

