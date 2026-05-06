       *> { dg-do run }
       *> { dg-output-file "group2/Preserve_collation_past_a_CALL.out" }
        identification          division.
        program-id.             asciibet.
        environment             division.
        configuration           section.
        special-names.
            alphabet like-ascii  is standard-1.
        object-computer.
            linux-system program collating sequence is like-ascii.
        data                    division.
        working-storage         section.
        01.
         02 signature pic x(8) value "76543210".
        procedure               division.
            display "Should collate like ASCII"
            if 'A' < 'a'
                display "Collates like ASCII"
            else
                display "Collates like EBCDIC"
            end-if
            call "ebcdicbet"
            display "Should collate like ASCII"
            if 'A' < 'a'
                display "Collates like ASCII"
            else
                display "Collates like EBCDIC"
            end-if
            goback.
        end program             asciibet.

        identification          division.
        program-id.             ebcdicbet.
        environment             division.
        configuration           section.
        special-names.
            alphabet like-ebcdic is EBCDIC.
        object-computer.
            linux-system program collating sequence is like-ebcdic.
        data                    division.
        working-storage         section.
        01.
         02 signature pic x(8) value "76543210".
        procedure               division.
            display "Should collate like EBCDIC"
            if 'A' < 'a'
                display "Collates like ASCII"
            else
                display "Collates like EBCDIC"
            end-if
            goback.
        end program             ebcdicbet.

