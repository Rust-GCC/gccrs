       *> { dg-do run }
       *> { dg-output-file "group2/compare_national_to_display.out" }
        identification      division.
        program-id.         prog.
        environment division.
        configuration section.
        special-names.
            locale greek is "cyrillic"
            locale unicode is "utf16le".
        object-computer.
            xerox-parc-star
            character classification
                for alphanumeric is greek
                for national is unicode.
        data                division.
        working-storage     section.
        01 aaa pic x(16).
        01 bbb pic n(16).
        procedure           division.
            move  "aaa" to aaa
            move n"aaa" to bbb
            if aaa equal bbb
                display "1 - correct"
            else
                display "1 -- bad"
                end-if
            if bbb equal aaa
                display "2 - correct"
            else
                display "2 -- bad"
                end-if
            move  "aaa" to aaa
            move n"bbb" to bbb
            if aaa < bbb
                display "3 - correct"
            else
                display "3 -- bad"
                end-if
            if bbb > aaa
                display "4 - correct"
            else
                display "4 -- bad"
                end-if
            move  "bbb" to aaa
            move n"aaa" to bbb
            if aaa > bbb
                display "5 - correct"
            else
                display "5 -- bad"
                end-if
            if bbb < aaa
                display "6 - correct"
            else
                display "6 -- bad"
                end-if
            goback.
        end program         prog.

