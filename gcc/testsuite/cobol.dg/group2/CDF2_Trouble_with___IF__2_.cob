       *> { dg-do run }
       *> { dg-output-file "group2/CDF2_Trouble_with___IF__2_.out" }

      *> This should compile, and doesn't
        identification division.
        program-id. prog2
        >>IF skip-init  IS DEFINED
        IS INITIAL .
        >>END-IF
        data division.
        working-storage section.
        77  VAR     INDEX.
        procedure division.
        set VAR TO +1
        display var
        set VAR TO -1000
        display var
        .
        end program prog2.

