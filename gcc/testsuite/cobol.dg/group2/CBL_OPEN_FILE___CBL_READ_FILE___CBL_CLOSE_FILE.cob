       *> { dg-do run }
       *> { dg-options "-dialect mf" }
       *> { dg-output-file "group2/CBL_OPEN_FILE___CBL_READ_FILE___CBL_CLOSE_FILE.out" }

        identification division.
        program-id. uat_cbl_read_file.

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
        01  access-mode pic x comp-x.
        01  deny-mode   pic x comp-x.
      *> Reserved value.
        01  device      pic x comp-x value 0.
        01  file-handle pic x(4) comp-5 value 4294967295.
        01  file-offset pic x(8) comp-x.
        01  byte-count  pic x(4) comp-x.
        01  flags       pic x comp-x.
        78  buflen      value 16.
        01  buffer      pic x(buflen).
        01  iterator    pic 9(2).
        01  i           pic 9(2).

        procedure division.
      *> Open as read-only.
          move 1 to access-mode.
      *> Deny both read and write.
          move 0 to deny-mode.
          display "Opening /dev/zero as read-only".
          call "CBL_OPEN_FILE" using "/dev/zero"
                                     access-mode
                                     deny-mode
                                     device
                                     file-handle.
          if return-code is less than 0
            display "Failed to open /dev/zero"
            go to end-label
          end-if.

          move all x"ff" to buffer(1 : buflen).

          move 0 to file-offset.
          move buflen to byte-count.
      *> Standard read.
          move 0 to flags.
          call "CBL_READ_FILE" using file-handle
                                     file-offset
                                     byte-count
                                     flags
                                     buffer.

          if return-code is less than 0
            display "Failed to read " byte-count " bytes"
            go to end-label
          end-if.

          perform varying i from 1 by 1 until i > buflen
            if buffer(i : 1) is not equal to x"00"
              display "Unexpected non-zero contents in buffer position "
                i ": " buffer(i : 1)
              go to end-label
            end-if
          end-perform.

          end-label.
          if file-handle is greater than 0 or equal to 0
            call "CBL_CLOSE_FILE" using file-handle
          end-if.
          goback.

