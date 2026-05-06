       *> { dg-do run }

      *> This compiles correctly; there should be no period after "prog"
      *> and there should be a period after INITIAL.  But, IS INITIAL is
      *> excluded because skip-init is not defined. 
        identification division.
        program-id. prog.
        >>IF skip-init  IS DEFINED
        IS INITIAL
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
        end program prog.

