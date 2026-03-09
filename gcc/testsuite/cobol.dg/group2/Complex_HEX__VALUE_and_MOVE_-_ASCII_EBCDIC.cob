       *> { dg-do run }
       *> { dg-output-file "group2/Complex_HEX__VALUE_and_MOVE_-_ASCII_EBCDIC.out" }
        identification division.
        program-id. hex-init.
        data division.
        working-storage section.
        01 template.
            05 under-test   pic x(8).
            05 filler       pic x.
            05 msg pic      x(12).
            05 filler       pic x.
            05 ascii-val    pic x(8).
            05 filler       pic x.
            05 ebcdic-val   pic x(8).

        01  var01020304.
            05 filler1.
                10 filler2     pic x(2) VALUE "33".
                10 as-value    pic x(4) VALUE  X'01020304'.
                10 filler3     pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var01020304".
            05 filler       pic x       value space.
            05 ascii-val    pic x(8)    value X"3333010203043333".
            05 filler       pic x       value space.
            05 ebcdic-val   pic x(8)    value X"f3f301020304f3f3".

        01  var-low.
            05 filler1.
                10 filler2      pic x(2) VALUE "33".
                10 as-value     pic x(4) VALUE  LOW-VALUES.
                10 filler3      pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var-low".
            05 filler       pic x       value space.
            05 ascii-val    pic x(8)    value X"3333000000003333".
            05 filler       pic x       value space.
            05 ebcdic-val   pic x(8)    value X"f3f300000000f3f3".

        01  var-space.
            05 filler1.
                10 filler2      pic x(2) VALUE "33".
                10 as-value     pic x(4) VALUE  SPACE.
                10 filler3      pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var-space".
            05 filler       pic x       value space.
            05 ascii-val    pic x(8)    value X"3333202020203333".
            05 filler       pic x       value space.
            05 ebcdic-val   pic x(8)    value X"f3f340404040f3f3".

        01  var-quote.
            05 filler1.
                10 filler2      pic x(2) VALUE "33".
                10 as-value     pic x(4) VALUE  QUOTE.
                10 filler3      pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var-quote".
            05 filler       pic x       value space.
            05 ascii-val    pic x(8)    value X"3333222222223333".
            05 filler       pic x       value space.
            05 ebcdic-val   pic x(8)    value X"f3f37f7f7f7ff3f3".

        01  var-zero.
            05 filler1.
                10 filler2      pic x(2) VALUE "33".
                10 as-value     pic x(4) VALUE  ZERO.
                10 filler3      pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var-zero".
            05 filler       pic x       value space.
            05 ascii-val    pic x(8)    value X"3333303030303333".
            05 filler       pic x       value space.
            05 ebcdic-val   pic x(8)    value X"f3f3f0f0f0f0f3f3".

        01  var-high.
            05 filler1.
                10 filler2      pic x(2) VALUE "33".
                10 as-value     pic x(4) VALUE  HIGH-VALUES.
                10 filler3      pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var-high".
            05 filler       pic x       value space.
            05 ascii-val    pic x(8)    value X"3333ffffffff3333".
            05 filler       pic x       value space .
            05 ebcdic-val   pic x(8)    value X"f3f3fffffffff3f3".

        procedure division.
            move var01020304  to template perform checker
            move var-low      to template perform checker
            move var-space    to template perform checker
            move var-quote    to template perform checker
            move var-zero     to template perform checker
            move var-high     to template perform checker
            goback.
        checker.
            display msg of template space with no advancing
            if under-test of template =
                        ascii-val of template
                     or ebcdic-val of template
                display "is okay."
            else
                display "is no good: " function hex-of(under-test)
                end-if
            continue.
        end program hex-init.

