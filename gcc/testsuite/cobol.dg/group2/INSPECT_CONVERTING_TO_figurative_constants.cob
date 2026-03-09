       *> { dg-do run }
       *> { dg-output-file "group2/INSPECT_CONVERTING_TO_figurative_constants.out" }
        identification division.
        program-id. clouseau.
        data division.
        working-storage section.
        01 item pic x(12).
        01 pitem redefines item pointer.

        01 l  pic x(8) value low-value.
        01 lp redefines l pointer.
        01 s pic x(8) value space.
        01 sp redefines s pointer.
        01 z pic x(8) value zero.
        01 zp redefines z pointer.
        01 q pic x(8) value quote.
        01 qp redefines q pointer.
        01 h pic x(8) value high-value.
        01 hp redefines h pointer.

        procedure division.
        move all "abcd" to item
        inspect item converting "abcd" to low-values
        display "low-values  " with no advancing
        if pitem = lp display "okay" else display "no good" end-if
        move all "abcd" to item
        inspect item converting "abcd" to spaces
        display "spaces      " with no advancing
        if pitem = sp display "okay" else display "no good" end-if
        move all "abcd" to item
        inspect item converting "abcd" to zeros
        display "zeros       " with no advancing
        if pitem = zp display "okay" else display "no good" end-if
        move all "abcd" to item
        inspect item converting "abcd" to quotes
        display "quotes      " with no advancing
        if pitem = qp display "okay" else display "no good" end-if
        move all "abcd" to item
        inspect item converting "abcd" to high-values
        display "high-values " with no advancing
        if pitem = hp display "okay" else display "no good" end-if
        goback.
        end program clouseau.

