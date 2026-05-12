       *> { dg-do run }
       *> { dg-options "-dialect ibm" }
       *> { dg-output-file "group2/RETURN-CODE_with_INITIAL_and_RECURSIVE.out" }
        identification      division.
        program-id.         prog.
        procedure           division.
            display "prog starting return code should be +0000:           " return-code
            call "prog-a"
            display "prog first return code from prog-a  should be +0123: " return-code
            call "prog-a"
            display "prog second return code from prog-a should be +0246: " return-code

            call "prog-i"
            display "prog first return code from prog-i  should be +0321: " return-code
            call "prog-i"
            display "prog second return code from prog-i should be +0321: " return-code

            call "prog-r"
            display "prog first return code from prog-r  should be -0123: " return-code
            call "prog-r"
            display "prog second return code from prog-r should be -0123: " return-code

            move zero to return-code
            goback.
        end program         prog.

        identification      division.
        program-id.         prog-a.
        procedure           division.
            add 123 to return-code
            goback.
        end program         prog-a.

        identification      division.
        program-id.         prog-i INITIAL.
        procedure           division.
            add 321 to return-code
            goback.
        end program         prog-i.

        identification      division.
        program-id.         prog-r RECURSIVE.
        procedure           division.
            add -123 to return-code
            goback.
        end program         prog-r.

