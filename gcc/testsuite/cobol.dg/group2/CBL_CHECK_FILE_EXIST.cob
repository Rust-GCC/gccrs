       *> { dg-do run }
       *> { dg-options "-dialect mf" }
       *> { dg-output-file "group2/CBL_CHECK_FILE_EXIST.out" }

        identification division.
        program-id. test_cbl_check_file_exist.
        environment division.
        configuration section.
          source-computer. Posix
        >>if debugging-mode is defined
          with debugging mode
        >>end-if
        .
          object-computer. Posix.
        data division.
        working-storage section.
        copy "cbltypes.cpy".
        01 buf type cblt-fileexist-buf.
        01 status-code pic x(2) comp-5.

        >> define GOOD as "/dev/null"
        >> define BAD as "/dev/thisfiledoesnotexist"
        procedure division.
        main section.
          perform open-good.
          perform open-bad.
          goback.

        open-good section.
          display "Checking whether " GOOD " exists".
          call "CBL_CHECK_FILE_EXIST" using GOOD
                                            buf
                                      returning status-code.
          if status-code is not zero
            display "CBL_CHECK_FILE_EXIST " GOOD " failed with "
              return-code
          else
            display "CBL_CHECK_FILE_EXIST " GOOD " was successful"
      *> The values below are returned by CBL_CHECK_FILE_EXIST and are
      *> inherently dynamic, so they cannot be tested reliably.
      D      display "cblte-fe-filesize is " cblte-fe-filesize
      D      display "cblte-fe-day is " cblte-fe-day
      D      display "cblte-fe-month is " cblte-fe-month
      D      display "cblte-fe-year is " cblte-fe-year
      D      display "cblte-fe-hours is " cblte-fe-hours
      D      display "cblte-fe-minutes is " cblte-fe-minutes
      D      display "cblte-fe-seconds is " cblte-fe-seconds
      D      display "cblte-fe-hundreths is " cblte-fe-hundreths
          end-if.
          exit paragraph.

        open-bad section.
          display "Checking whether " BAD " exists".
          call "CBL_CHECK_FILE_EXIST" using BAD
                                            buf
                                      returning status-code.
          if status-code is not zero
            display "Expected failure: CBL_CHECK_FILE_EXIST " BAD
          else
            display "CBL_CHECK_FILE_EXIST " BAD
              " was unexpectedly successful"
          end-if.
          exit paragraph.

