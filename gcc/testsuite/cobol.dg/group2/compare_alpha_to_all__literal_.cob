       *> { dg-do run }
       *> { dg-output-file "group2/compare_alpha_to_all__literal_.out" }
        identification      division.
        program-id.         prog.
        data                division.
        working-storage     section.
        01 var1 pic x(64) value all "Bob".
        procedure           division.
            if var1 equal all "Bob"
                display "It's all Bob."
            else
                display "It's not Bob."
                end-if
            goback.
        end program         prog.

