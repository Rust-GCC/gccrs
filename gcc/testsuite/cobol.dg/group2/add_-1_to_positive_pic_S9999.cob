       *> { dg-do run }
       *> { dg-output-file "group2/add_-1_to_positive_pic_S9999.out" }
        *> We have special code for adding single digits to
        *> numeric-display variables.  This routine checks adding a negative
        *> value to positive signable targets.
        identification              division.
        program-id.                 prog.
        data                        division.
        working-storage             section.
        01 foo1 pic s9999.
        01 foo2 pic s99v99.
        procedure                   division.
        move 2 to foo1
        add -1 to foo1 display foo1 " should be +0001"
        add -1 to foo1 display foo1 " should be +0000"
        add -1 to foo1 display foo1 " should be -0001"
        move 2.21 to foo2           
        add -1 to foo2 display foo2 " should be +01.21"
        add -1 to foo2 display foo2 " should be +00.21"
        add -1 to foo2 display foo2 " should be -01.79"
            goback.
        end program                 prog.

