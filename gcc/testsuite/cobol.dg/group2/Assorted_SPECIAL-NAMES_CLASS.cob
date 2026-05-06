       *> { dg-do run }
       *> { dg-output-file "group2/Assorted_SPECIAL-NAMES_CLASS.out" }
        identification      division.
        program-id.         nat.
        environment         division.
        configuration       section.
        special-names.
            *> Note that working with numeric collation positions rather than
            *> text characters gets extremely confusing and tricky in our
            *> current paradigm, which is to convert everything to UTF32 in
            *> order to make comparisons when things like multi-byte UTF-8
            *> encoding is involved.  Likewise the possibility of working in
            *> UTF-16 and encountering double-word encodings.  It's not easy
            *> to declare what is "right".  This code works with ASCII, EBCDIC,
            *> and UTF-8 as the ALPHANUMERIC/DISPLAY encoding.

            locale unicode   is "utf16le"
            CLASS HexNumber  IS "0" THRU "9", "A" THRU "F", 
                                               "a" THRU "f"
            CLASS RealName   IS "A" THRU "Z", 
                                "a" THRU "z"
            CLASS JustJ      IS "J"
            CLASS AsciiZero  IS 49
            CLASS EbcdicZero IS 49
            CLASS LooseDigit IS 49 THROUGH 58 241 THROUGH 250
            CLASS IntChars   IS "INJMLK"
            .
        object-computer.
            gnu-linux
            classification for national is unicode
            .
        data division.
        working-storage     section.
            01 J          pic X    value "J".
            01 K          pic X    value "K".
            01 S          pic X    value "S".
            01 hex-value  pic X(4) value "FF00".
            01 name       pic X(9) value "AOMalleyz".
            01 zed        pic X    value "0".
            01 four       pic X    value "4".
        procedure           division.
            if J is JustJ 
                display "properly J IS J"
            else 
                display "IMPROPERLY NOT J IS J" 
                end-if

            if K is JustJ 
                display "IMPROPERLY K IS J" 
            else 
                display "properly not K IS J" 
                end-if

            if hex-value is HexNumber 
                display "properly hex-value IS HexNumber" 
            else 
                display "IMPROPERLY not hex-value IS HexNumber" 
                end-if

            if name is HexNumber 
                display "IMPROPERLY name IS HexNumber" 
            else 
                display "properly not name IS HexNumber" 
                end-if

            if name is RealName 
                display "properly name IS RealName" 
            else 
                display "IMPROPERLY not name IS RealName" 
                end-if

            if zed is EbcdicZero 
                display "properly zed IS EbcdicZero" 
            else 
                display "IMPROPERLY not zed IS EbcdicZero" 
                end-if
            if zed is AsciiZero 
                display "properly zed IS AsciiZero" 
            else 
                display "IMPROPERLY not zed IS AsciiZero" 
                end-if

            if four is LooseDigit 
                display "properly four IS LooseDigit" 
            else 
                display "IMPROPERLY not four IS LooseDigit" 
                end-if

            if J is LooseDigit 
                display "IMPROPERLY J IS LooseDigit" 
            else 
                display "properly not J IS LooseDigit" 
                end-if

            if J is IntChars 
                display "properly J IS IntChars" 
            else 
                display "IMPROPERLY not J IS IntChars" 
                end-if

            if S is IntChars 
                display "IMPROPERLY S IS IntChars" 
            else 
                display "properly not S IS IntChars" 
                end-if

            goback.
            end program         nat.

