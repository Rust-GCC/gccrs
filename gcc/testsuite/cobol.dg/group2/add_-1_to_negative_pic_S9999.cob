       *> { dg-do run }
       *> { dg-output-file "group2/add_-1_to_negative_pic_S9999.out" }
        *> We have special code for adding single digits to
        *> numeric-display variables.  This routine checks adding a negative
        *> value to negative signable targets.
        identification              division.
        program-id.                 prog.
        data                        division.
        working-storage             section.
        01 foo1 pic s9.
        01 foo4 pic s9999.
        01 foov pic s9v99.
        procedure                   division.
        move -8 to foo1
        add -1 to foo1 display foo1 
        add -1 to foo1 display foo1
        add -1 to foo1 display foo1
        move -8 to foo1
        add -5 to foo1 display foo1 
        move -9998 to foo4
        add -1 to foo4 display foo4
        add -1 to foo4 display foo4
        add -1 to foo4 display foo4
        move -9998 to foo4
        add -5 to foo4 display foo4 
        move -9998 to foov
        add -1 to foov display foov 
        add -1 to foov display foov
        add -1 to foov display foov
        move -98.21 to foov
        add -1 to foov display foov 
        add -1 to foov display foov
        add -1 to foov display foov
            goback.
        end program                 prog.

