       *> { dg-do run }
       *> { dg-output-file "group2/ENTRY_statement.out" }
        identification          division.
        program-id.             prog.
        data                    division.
        working-storage         section.
        01 msg pic x(32).
        procedure               division.
            move "This is foo" to msg
            display "About to call FOO"
            call    "foo" using msg
            move "This is bar" to msg
            display "About to call BAR"
            call    "bar" using msg
            move "This is foo2" to msg
            display "About to call FOO again"
            call    "foo" using msg
            goback.
            end program         prog.

        identification          division.
        program-id.             foo.
        data                    division.
        linkage                 section.
        01 msg pic x(32).
        procedure               division using msg.
            display "  entry point foo: " function trim (msg)
            entry   "bar"
            display "  entry point bar: " function trim (msg)
            goback.
            end program         foo.

