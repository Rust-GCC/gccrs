       *> { dg-do run }
       *> { dg-output-file "group2/N-Queens_algorithm.out" }
        identification division.
        program-id. xdamcnt.
        data division.
        working-storage section.
        77 i             pic 9(8) comp-5.
        77 j             pic 9(8) comp-5.
        77 k             pic 9(8) comp-5.
        77 n             pic 9(8) comp-5.
        77 n2            pic 9(8) comp-5.
        77 l             pic s9(8) comp-5.
        77 z             pic 9(8) comp-5.
        77 configokret   pic 9(8) comp-5.
        01 a_vector.
        10  a         pic 9(8) comp-5 occurs 100 value 0.
        77 istart        pic 9(8) comp-5 value 1.
        77 iend          pic 9(8) comp-5 value 0.
        77 cnt           pic 9(8) comp-5 value 0.
        77 slen          pic 9(8) comp-5 value 0.
        77 argc          pic 9(8) comp-5 value 0.
        77 argv          pic x(100) value spaces.

        procedure division.
        pmain section.
            display "N-queens problem in COBOL".
            display '  2   4    6     8      10         12           14'.
            display '1 0 0 2 10 4 40 92 352 724 2680 14200 73712 365596'. 

            move 11 to iend
            move 1 to istart

            display "istart=", istart, " iend=", iend.

            perform varying i from istart by 1 until i > iend
                perform nqsolve
                display "d(", i, ") = ", cnt
                end-perform.

            goback.

        *> Calculate number of positions for n queens.
        nqsolve section.
            move zero to cnt.
            move 1 to k.
            move 1 to a(1).
            move i to n.
            move i to n2.

        lloop.
            perform configok.
            if configokret = 1 then
                if k < n then
                    add 1 to k
                    move 1 to a(k)
                    go to lloop
                else
                    add 1 to cnt
                    end-if
                end-if.

        perform with test after varying k from k by -1 until k <= 1
            if a(k) < n then
                add 1 to a(k)
                go to lloop
                end-if
            end-perform.

            add 1 to a(1).
            if a(1) > n2 then
                exit section
                end-if.
            move 2 to k.
            move 1 to a(2).
            go to lloop.

        *> check if k-th queen is attacked by any other prior queen.
        *> return nonzero if configuration is ok, zero otherwise.
        configok section.
            move zero to configokret.
            move a(k) to z.

            perform varying j from 1 by 1 until j >= k
                compute l = z - a(j)
                if l = 0 then
                    exit section
                    end-if
            if l < 0 then
                compute l = 0 - l
                end-if
            if l = k - j then
                exit section
                end-if
            end-perform.

        move 1 to configokret.

        dummy section.
            display space.

        end program xdamcnt.

