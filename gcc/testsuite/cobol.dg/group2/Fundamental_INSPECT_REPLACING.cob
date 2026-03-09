       *> { dg-do run }
       *> { dg-options "-dialect mf" }
       *> { dg-output-file "group2/Fundamental_INSPECT_REPLACING.out" }
        program-id.         prog.
        data                division.
        working-storage     section.
        *> Note that 'item' has to have a length divisible by three for
        *> the trailing "Abc" tests to work.
        01 item pic x(45).
        procedure division.
        display "INSPECT REPLACING Abc by MMM"
        move "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc" to item
        display function trim(item)

        move "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc" to item
        inspect item replacing all "Abc" by "MMM"
        display "all          " item with no advancing
        if item <> "MMMMMMXMMMMMMMMMYMMMMMMMMMMMM MMMMMMMMMMMMMMM" 
                    display " Not right." else display " Okay." end-if

        move "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc" to item
        inspect item replacing leading "Abc"  by "MMM"
        display "leading      " item with no advancing
        if item <> "MMMMMMXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc"
                    display " Not right." else display " Okay." end-if

        move "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc" to item
        inspect item replacing all "Abc"  by "MMM" after "X" before "Y"
        display "X to Y       " item with no advancing
        if item <> "AbcAbcXMMMMMMMMMYAbcAbcAbcAbc AbcAbcAbcAbcAbc"
                    display " Not right." else display " Okay." end-if

        move "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc" to item
        inspect item replacing all "Abc"  by "MMM" after "Y" before space
        display "Y to space   " item with no advancing
        if item <> "AbcAbcXAbcAbcAbcYMMMMMMMMMMMM AbcAbcAbcAbcAbc"
                    display " Not right." else display " Okay." end-if

        move "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc" to item
        inspect item replacing trailing "Abc" by "MMM"
        display "trailing     " item with no advancing
        if item <> "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc MMMMMMMMMMMMMMM "
                    display " Not right." else display " Okay." end-if

        move "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc" to item
        inspect item replacing all "Abc"  by "MMM" after "X"
        display "after X      " item with no advancing
        if item <> "AbcAbcXMMMMMMMMMYMMMMMMMMMMMM MMMMMMMMMMMMMMM "
                    display " Not right." else display " Okay." end-if

        move "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc" to item
        inspect item replacing all "Abc" by "MMM" before space
        display "before space " item with no advancing
        if item <> "MMMMMMXMMMMMMMMMYMMMMMMMMMMMM AbcAbcAbcAbcAbc"
                    display " Not right." else display " Okay." end-if

        move "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc" to item
        inspect item replacing all "Abc" by "MMM" after "b"
        display "after b      " item with no advancing
        if item <> "AbcMMMXMMMMMMMMMYMMMMMMMMMMMM MMMMMMMMMMMMMMM"
                    display " Not right." else display " Okay." end-if

        move "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc" to item
        inspect item replacing all "Abc"  by "MMM" before "b"
        display "before b     " item with no advancing
        if item <> "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc"
                    display " Not right." else display " Okay." end-if

        goback.

        end program prog.

