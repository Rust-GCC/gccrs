       *> { dg-do run }
       *> { dg-output-file "group2/add_-1_to_pic_9999.out" }
        *> We have special code for adding single digits to
        *> numeric-display variables.  This routine checks adding a negative
        *> value to unsignable targets.
        identification              division.
        program-id.                 prog.
        data                        division.
        working-storage             section.
        01 foo1 pic 9.
        01 foo4 pic 9999.
        01 foov pic 9v99.
        procedure                   division.
        move 2 to foo1
        add -1 to foo1 display foo1 
        add -1 to foo1 display foo1
        add -1 to foo1 display foo1
        move 3 to foo1
        add -5 to foo1 display foo1 
        move 2 to foo4
        add -1 to foo4 display foo4
        add -1 to foo4 display foo4
        add -1 to foo4 display foo4
        move 3 to foo4
        add -5 to foo4 display foo4 
        move 2 to foov
        add -1 to foov display foov 
        add -1 to foov display foov
        add -1 to foov display foov
        move 2.21 to foov
        add -1 to foov display foov 
        add -1 to foov display foov
        add -1 to foov display foov
            goback.
        end program                 prog.

