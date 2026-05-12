       *> { dg-do run }
       *> { dg-output-file "group2/ENTRY_statement.out" }
        identification          division.
        program-id.             prog.
        data                    division.
        working-storage         section.
        01 msg pic x(32).
        procedure               division.
            move "This is FOO" to msg
            display "About to call FOO"
            call    "FOO" using msg
            move "This is BAR" to msg
            display "About to call BAR"
            call    "BAR" using msg
            move "This is FOO2" to msg
            display "About to call FOO again"
            call    "FOO" using msg

            move "This is BAZZ" to msg
            display "About to call BAZZ"
            call    "BAZZ" using msg
            move "This is FAZZ" to msg
            display "About to call FAZZ"
            call    "FAZZ" using msg
            move "This is baz2" to msg
            display "About to call BAZZ again"
            call    "BAZZ" using msg

            goback.
            end program         prog.

        identification          division.
        program-id.             FOO.
        data                    division.
        linkage                 section.
        01 msg pic x(32).
        procedure               division using msg.
            display "  entry point FOO: " function trim (msg)
            entry   "BAR"
            display "  entry point BAR: " function trim (msg)
            goback.
            entry   "BAZZ"
            display "  entry point BAZZ: " function trim (msg)
            entry   "FAZZ"
            display "  entry point FAZZ: " function trim (msg)
            goback.
            end program         FOO.

