      * Copyright (c) 2021-2026 Symas Corporation
      *
      * Redistribution and use in source and binary forms, with or without
      * modification, are permitted provided that the following conditions are
      * met:
      *
      * * Redistributions of source code must retain the above copyright
      *   notice, this list of conditions and the following disclaimer.
      * * Redistributions in binary form must reproduce the above
      *   copyright notice, this list of conditions and the following disclaimer
      *   in the documentation and/or other materials provided with the
      *   distribution.
      * * Neither the name of the Symas Corporation nor the names of its
      *   contributors may be used to endorse or promote products derived from
      *   this software without specific prior written permission.
      *
      * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
      * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
      * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
      * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
      * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
      * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
      * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
      * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
      * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
      * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
      * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
        COPY posix-errno.
        COPY psx-open.

       IDENTIFICATION DIVISION.
       PROGRAM-ID. gcobol-smoke-test.

       ENVIRONMENT DIVISION.
       CONFIGURATION SECTION.
       SOURCE-COMPUTER.
           GNU-Linux.
       OBJECT-COMPUTER.
           GNU-Linux.

       >>Define FILENAME as "/tmp/smoke.empty"

       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
           SELECT EXPENDABLE
                  ACCESS MODE IS SEQUENTIAL
                  SEQUENTIAL
                  ASSIGN TO FILENAME.

       DATA DIVISION.
       FILE SECTION.
      * FD not required per ISO but fails under gcobol.
        FD EXPENDABLE.
          01 Extraneous PIC X.

       WORKING-STORAGE SECTION.
        77 File-Name   PIC X(100) VALUE FILENAME.
        77 status-code BINARY-LONG SIGNED.

      * CBL_ALLOC_MEM
        01 mem-pointer	usage pointer.
        77 mem-size	pic x(8) comp-5 VALUE 64.
        77 flags        pic x(8) comp-5 VALUE 0.

      * CBL_CHECK_FILE_EXIST
        01 file-info.
          03 file-modification-day.
            05 File-Size-In-Bytes  PIC 9(18)  COMP.
            05 Mod-DD              PIC 9(2)   COMP. *> Modification Date
            05 Mod-MO              PIC 9(2)   COMP.
            05 Mod-YYYY            PIC 9(4)   COMP.
          03 file-modification-time.
            05 Mod-HH              PIC 9(2)   COMP. *> Modification Time
            05 Mod-MM              PIC 9(2)   COMP.
            05 Mod-SS              PIC 9(2)   COMP.
            05 FILLER              PIC 9(2)   COMP. *> Always 00

      * CBL_OPEN_FILE
        77  access-mode            PIC 9(8) COMP-5.
        77  deny-mode              PIC 9(8) COMP-5.
        77  File-Name2             PIC X(100) VALUE FILENAME.
        77  device                 PIC X COMP-X VALUE 0.
        77  file-handle            PIC X(4) COMP-5.

      * CBL_READ_FILE
       >>Define MAX_FILE_SIZE as 50000
        77  WS-BYTE-CNT            Binary-Long.
        77 File-Buffer             PIC X(MAX_FILE_SIZE).
        77 File-Offset             PIC S9(8) COMP-5.
        77 File-Flags              PIC X COMP-X VALUE 0.
        77 File-COUNT              PIC 9(8) COMP-5 VALUE MAX_FILE_SIZE.

      * CBL_CREATE_FILE
        77  File-Name3             PIC X(100) VALUE FILENAME.
        77  filename-len           PIC 9(4) BINARY VALUE ZERO.
        77  errno-val              Binary-Long.
      * CBL_WRITE_FILE
        01 Actual-Data-Len         PIC 9(4) BINARY VALUE ZERO.
        01 Newline                 PIC X VALUE X"0A".

       PROCEDURE DIVISION.

        Display 'Allocating ' mem-size ' bytes ... ' with No Advancing.

        Call "CBL_ALLOC_MEM" using
                               mem-pointer
                     by value  mem-size
                     by value  flags
                     returning status-code.

        Display 'CBL_ALLOC_MEM        status: ' status-code.

        Display 'Checking on  ' Function Trim(File-Name)
          ' ...                 ' with No Advancing.

        Call "CBL_CHECK_FILE_EXIST"  using    File-Name
                                     file-info
                           returning status-code.

        Display 'CBL_CHECK_FILE_EXIST status: ' status-code.

        Display 'Deleting     ' Function Trim(File-Name)
          ' ...                 ' with No Advancing.

        Call "CBL_DELETE_FILE" using File-Name
                     returning status-code.

        Display 'CBL_DELETE_FILE      status: ' status-code.

        Display 'Freeing ' mem-size ' bytes ...    ' with No Advancing.

        Call "CBL_FREE_MEM" using by value mem-pointer
                    returning      status-code.

        Display 'CBL_FREE_MEM         status: ' status-code.

      * Insert new tests for open, read and write file:
        Move S_IRWXU TO deny-mode.
      *  Move 1 to access-mode.  *> read only
      *  Move 65 to access-mode. *> read only
      *  Move 2 to access-mode.  *> write only
      *  Move 67 to access-mode.  *> write only
        Move 3 to access-mode.  *> read/write
      *  Move 67 to access-mode.  *> read/write
        Move "/tmp/foo5.txt" to File-Name2.
      * >>IF CBL_OPEN_FILE is defined
        Call "CBL_OPEN_FILE" using File-Name2, access-mode, deny-mode,
          device, file-handle returning status-code.
      * >>END-IF
        Display 'CBL_OPEN_FILE file-handle: ' file-handle
          ', status-code: ' status-code.

      * >>IF CBL_READ_FILE is defined

        If status-code = 0
        Then
      * Get input file size:
          Move 128 to File-Flags
          Move 0 to File-Offset
          perform DO_READ.
        If status-code = 0
        Then
          MOVE File-Offset to File-COUNT
          Display 'CBL_READ_FILE size: ' File-COUNT
        Else
          Display 'CBL_READ_FILE Cannot determine file size or empty'
        End-if.

      * Read the file:
        If status-code = 0
        Then
          Move 0 to File-Flags
          Move 0 to File-Offset
          perform DO_READ

          Move status-code to File-Offset
          Display 'Do 2nd read: '
          perform DO_READ
        End-if.
      * >>END-IF

      * >>IF CBL_CREATE_FILE is defined
        Move "/tmp/foo7.txt" to File-Name3.
        Move -1 to file-handle.
        perform DO_CREATE.
      * >>END-IF

      * >>IF CBL_WRITE_FILE is defined
        MOVE SPACES TO File-Buffer
        MOVE
            "abcdefghijklmnopqrstuvwxyz" &
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ" &
            "0123456789" &
            "~!@#$%^&*()_+[]{};:,<.>/?~`"
        TO File-Buffer.

        Inspect File-Buffer Tallying Actual-Data-Len For Characters
          Before Initial " ".
        Display "file buffer Actual data length: " Actual-Data-Len.
        Add 1 TO Actual-Data-Len.
        Move Newline TO File-Buffer (Actual-Data-Len:1).
        Move Actual-Data-Len to File-COUNT.
        Move Actual-Data-Len to File-COUNT.

      * normal write:
        perform DO_WRITE.
      * test truncate:
      *  Move 0 to File-Count.
      *  Move 100 to File-Offset.
      *  perform DO_WRITE.

      * test extend:
      *  Move 1000 to File-Offset.
      *  perform DO_WRITE.
      * >>END-IF

        Call "CBL_CHECK_FILE_EXIST"  using    File-Name2
                                     file-info
                           returning status-code.
        Display 'CBL_CHECK_FILE_EXIST 2 status: ' status-code.

      * create a new file empty file:=
      *  Move 0 to access-mode.  *> create
      *  Move S_IRWXU TO deny-mode.
      *  Move "/tmp/foo6.txt" to File-Name2.
      *  Call "CBL_OPEN_FILE" using File-Name2, access-mode, deny-mode,
      * device, file-handle
      *   returning status-code.

      * write to it:
      *  MOVE SPACES TO File-Buffer
      *  Move "Every Good Programmer Deserves COBOL." to File-Buffer.
      *  Inspect File-Buffer Tallying Actual-Data-Len For Characters
      *   Before Initial " ".
      *  Display "file buffer Actual data length: " Actual-Data-Len.
      *  Add 1 TO Actual-Data-Len.
      *  Move Newline TO File-Buffer (Actual-Data-Len:1).
      *  Move Actual-Data-Len to File-COUNT.
      *  Move 37 to File-COUNT.
      * normal write:
      *  perform DO_WRITE.

      * read it:

        GOBACK.

       DO_CREATE.
      * TODO: this probably should be handled in the API:
           COMPUTE filename-len = FUNCTION
            LENGTH(FUNCTION TRIM(File-Name3)).
           MOVE X"00" TO File-Name3(filename-len + 1:1).

           Call "CBL_CREATE_FILE"
             using File-Name3, access-mode, deny-mode, device,
               file-handle
             returning status-code.
           if return-code is less than 0
             call "posix_errno" using File-Name returning errno-val
             display "CBL_CREATE_FILE failed with errno: " errno-val.

           Display 'CBL_CREATE_FILE status-code: ' status-code
            ', file-handle: ' file-handle.
           exit.

       DO_WRITE.
           Call "CBL_WRITE_FILE"
             using file-handle, File-Offset, File-COUNT, File-Flags,
              By reference File-Buffer
             returning status-code.
           Display 'CBL_WRITE_FILE status-code: ' status-code
           ', buffer [' Function Trim(File-Buffer) ']'.
           exit.

       DO_READ.
           MOVE SPACES TO File-Buffer
           Call "CBL_READ_FILE"
             using file-handle, File-Offset, By reference File-COUNT,
              File-Flags, By reference File-Buffer
           returning status-code.
           Display 'CBL_READ_FILE status-code: ' status-code
            ', buffer [' FUNCTION TRIM(File-Buffer)']'.
           exit.
