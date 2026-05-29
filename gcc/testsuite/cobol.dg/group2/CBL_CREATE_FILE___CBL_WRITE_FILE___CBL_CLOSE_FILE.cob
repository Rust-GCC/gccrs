       *> { dg-do run }
       *> { dg-options "-dialect mf" }

        identification division.
        program-id. test_cbl_write_file.

        environment division.
        configuration section.
          source-computer. Posix
        >>if debugging-mode is defined
          with debugging mode
        >>end-if
        .
          object-computer. Posix.

        >> define FILE_NAME as "test_cbl_write_file.cbl.txt"

        data division.
        working-storage section.
      *> Create and open as write-only.
        01  access-mode pic x comp-x value 2.
      *> Deny both read and write,
      *> As per specs, it must be assigned to zero on write-only.
      *> Not supported by GnuCOBOL or gcc-cobol, anyway.
        01  deny-mode   pic x comp-x value 0.
      *> Reserved value
        01  device      pic x comp-x value 0.
        01  file-handle pic x(4) comp-5.
        01  fh-signed   binary-long value -1.
        01  file-offset pic x(8) comp-x value 0.
      *> Standard write.
        01  flags       pic x comp-x value 0.
        01  byte-count  pic x(4) comp-x.
        01  buffer      pic x(5) value "hello".
        01  error-msg   pic x(128).

        procedure division.
      *> First, ensure the test file exists.
          call "CBL_CREATE_FILE" using FILE_NAME
                                       access-mode
                                       deny-mode
                                       device
                                       file-handle.

          if return-code <> 0
            display "CBL_CREATE_FILE " FILE_NAME " failed with "
              return-code
            go to end-label
          end-if.

          move function length(buffer) to byte-count.

          move file-handle to fh-signed.
          call "CBL_WRITE_FILE" using file-handle
                                      file-offset
                                      byte-count
                                      flags
                                      buffer.

          if return-code <> 0
            display "CBL_WRITE_FILE failed with " return-code
          end-if.

          end-label.
          if fh-signed is greater than 0
            call "CBL_CLOSE_FILE" using file-handle
          end-if.

          goback.

