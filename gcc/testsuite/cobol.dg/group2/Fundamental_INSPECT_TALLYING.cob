       *> { dg-do run }
       *> { dg-options "-dialect mf" }
       *> { dg-output-file "group2/Fundamental_INSPECT_TALLYING.out" }
        program-id.         prog.
        data                division.
        working-storage     section.
        *> Note that 'item' has to have a length divisible by three for
        *> the trailing "Abc" tests to work.
        01 item pic x(45).
        01 counter pic 999.
        procedure division.
        move "AbcAbcXAbcAbcAbcYAbcAbcAbcAbc AbcAbcAbcAbcAbc" to item
        display function trim(item)
        move zero to counter
        inspect item tallying counter for all "Abc" 
        display "All       ""Abc"" " counter " (014)"
        move zero to counter
        inspect item tallying counter for leading "Abc" 
        display "Leading   ""Abc"" " counter " (002)"
        move zero to counter
        inspect item tallying counter for all "Abc" after "X" before "Y"
        display "X to Y    ""Abc"" " counter " (003)"
        move zero to counter
        inspect item tallying counter for all "Abc" after "Y" before space
        display "Y to ' '  ""Abc"" " counter " (004)"
        move zero to counter
        inspect item tallying counter for trailing "Abc" 
        display "Trailing  ""Abc"" " counter " (005)"
        move zero to counter
        inspect item tallying counter for all "Abc" after "X"
        display """Abc"" after ""x"" " counter " (012)"
        move zero to counter
        inspect item tallying counter for all "Abc" before space
        display "before space    " counter " (009)"
        move zero to counter
        inspect item tallying counter for all "Abc" after "b"
        display """Abc"" after ""b"" " counter " (013)"
        move zero to counter
        inspect item tallying counter for all "Abc" before "b"
        display "before ""b""      " counter " (000)"
        goback.
        end program prog.

