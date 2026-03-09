       *> { dg-do run }
       *> { dg-output-file "group2/Simple_ENVIRONMENT-NAME_with_exception.out" }
        identification division.
        program-id. envtest.
        data division.
        working-storage section.
        01 ename        pic x(32).
        01 evalue       pic x(32).
        procedure division.
        move "USER" to ename
        display ename  upon environment-name
        accept  evalue from environment-value
            not on exception Display "We got a value back" end-display
            end-accept
        goback.
        end program envtest.

