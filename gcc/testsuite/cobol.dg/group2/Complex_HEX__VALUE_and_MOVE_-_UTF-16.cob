       *> { dg-do run }
       *> { dg-options "-fexec-charset=utf16le" }
       *> { dg-output-file "group2/Complex_HEX__VALUE_and_MOVE_-_UTF-16.out" }
        identification division.
        program-id. hex-init.
        data division.
        working-storage section.
        01 template.
            05 under-test   pic x(8).
            05 filler       pic x.
            05 msg pic      x(12).
            05 filler       pic x.
            05 utf16-val    pic x(8).

        01  var01020304.
            05 filler1.
                10 filler2     pic x(2) VALUE "33".
                10 as-value    pic x(4) VALUE  X'01020304'.
                10 filler3     pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var01020304".
            05 filler       pic x       value space.
            05 utf16-val    pic x(8)    value X"33003300010203040000000033003300".

        01  var-low.
            05 filler1.
                10 filler2      pic x(2) VALUE "33".
                10 as-value     pic x(4) VALUE  LOW-VALUES.
                10 filler3      pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var-low".
            05 filler       pic x       value space.
            05 utf16-val    pic x(8)    value X"33003300000000000000000033003300".

        01  var-space.
            05 filler1.
                10 filler2      pic x(2) VALUE "33".
                10 as-value     pic x(4) VALUE  SPACE.
                10 filler3      pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var-space".
            05 filler       pic x       value space.
            05 utf16-val    pic x(8)    value X"33003300200020002000200033003300".

        01  var-quote.
            05 filler1.
                10 filler2      pic x(2) VALUE "33".
                10 as-value     pic x(4) VALUE  QUOTE.
                10 filler3      pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var-quote".
            05 filler       pic x       value space.
            05 utf16-val    pic x(8)    value X"33003300220022002200220033003300".

        01  var-zero.
            05 filler1.
                10 filler2      pic x(2) VALUE "33".
                10 as-value     pic x(4) VALUE  ZERO.
                10 filler3      pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var-zero".
            05 filler       pic x       value space.
            05 utf16-val    pic x(8)    value X"33003300300030003000300033003300".

        01  var-high.
            05 filler1.
                10 filler2      pic x(2) VALUE "33".
                10 as-value     pic x(4) VALUE  HIGH-VALUES.
                10 filler3      pic x(2) VALUE "33".
            05 filler       pic x       value space.
            05 msg pic      x(12)       value "var-high".
            05 filler       pic x       value space.
            05 utf16-val    pic x(8)    value X"33003300FF00FF00FF00FF0033003300".

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
            if utf16-val of template =
                        utf16-val of template
                display "is okay."
            else
                display "is no good: " function hex-of(under-test)
                end-if
            continue.
        end program hex-init.

