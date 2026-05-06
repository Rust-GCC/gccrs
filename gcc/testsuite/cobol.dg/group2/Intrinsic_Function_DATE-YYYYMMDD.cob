       *> { dg-do run }
        identification division.
        program-id. prog.

        data division.
        working-storage section.

        01 datev     pic 99999999.
        01 should_be pic 9999.
        01 result    pic 9999.

        procedure division.

        move function test-date-yyyymmdd(19450601) to result
        move zero to should_be
        if result not equal to should_be then
            display "test-date-yyyymmdd(19450601) should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move function test-date-yyyymmdd(100000000) to result
        move 1 to should_be
        if result not equal to should_be then
            display "test-date-yyyymmdd(100000000) should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move 16010101 to datev
        move zero to should_be
        move function test-date-yyyymmdd(datev) to result
        if result not equal to should_be then
            display "test-date-yyyymmdd(" datev ") should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move 16010100 to datev
        move 3 to should_be
        move function test-date-yyyymmdd(datev) to result
        if result not equal to should_be then
            display "test-date-yyyymmdd(" datev ") should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move 99991231 to datev
        move zero to should_be
        move function test-date-yyyymmdd(datev) to result
        if result not equal to should_be then
            display "test-date-yyyymmdd(" datev ") should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move 99991232 to datev
        move 3 to should_be
        move function test-date-yyyymmdd(datev) to result
        if result not equal to should_be then
            display "test-date-yyyymmdd(" datev ") should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move 19450601 to datev
        move zero to should_be
        move function test-date-yyyymmdd(datev) to result
        if result not equal to should_be then
            display "test-date-yyyymmdd(" datev ") should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move 19450600 to datev
        move 3 to should_be
        move function test-date-yyyymmdd(datev) to result
        if result not equal to should_be then
            display "test-date-yyyymmdd(" datev ") should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move 19450631 to datev
        move 3 to should_be
        move function test-date-yyyymmdd(datev) to result
        if result not equal to should_be then
            display "test-date-yyyymmdd(" datev ") should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move 19450001 to datev
        move 2 to should_be
        move function test-date-yyyymmdd(datev) to result
        if result not equal to should_be then
            display "test-date-yyyymmdd(" datev ") should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move 19454701 to datev
        move 2 to should_be
        move function test-date-yyyymmdd(datev) to result
        if result not equal to should_be then
            display "test-date-yyyymmdd(" datev ") should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move 19880229 to datev
        move 0 to should_be
        move function test-date-yyyymmdd(datev) to result
        if result not equal to should_be then
            display "test-date-yyyymmdd(" datev ") should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        move 19890229 to datev
        move 3 to should_be
        move function test-date-yyyymmdd(datev) to result
        if result not equal to should_be then
            display "test-date-yyyymmdd(" datev ") should have been "
                    should_be " but was " result
            move 1 to return-code
            end-if.

        end program prog.

