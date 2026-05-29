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

      * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
      *
      * CALL "CBL_ALLOC_MEM" using     mem-pointer
      *                      by value  mem-size
      *                      by value  flags
      *                      returning status-code
      * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
       >>PUSH SOURCE FORMAT
       >>SOURCE FIXED
       COPY "cblproto.cpy".

       IDENTIFICATION DIVISION.
       PROGRAM-ID. CBL_ALLOC_MEM.

       ENVIRONMENT DIVISION.
       CONFIGURATION SECTION.
         SOURCE-COMPUTER. Posix
       >>IF DEBUGGING-MODE IS Defined
          With Debugging Mode
       >>END-IF
       .

       DATA DIVISION.

       LINKAGE SECTION.
       01  MEMORY-REQUESTED		PIC X(8) COMP-5.
       01  MEMORY-ALLOCATED		USAGE IS POINTER.
       01  FLAGS                	PIC X(8) COMP-5.
       77  STATUS-CODE                  PIC X(2) COMP-5 VALUE 0.

       PROCEDURE DIVISION USING     MEMORY-ALLOCATED,
                          BY VALUE  MEMORY-REQUESTED,
                          BY VALUE  FLAGS
                          RETURNING STATUS-CODE.

      D     Display 'MEMORY-REQUESTED: ' MEMORY-REQUESTED
      D            ' CHARACTERS INITIALIZED'

           ALLOCATE MEMORY-REQUESTED CHARACTERS
                    RETURNING MEMORY-ALLOCATED.

          IF MEMORY-ALLOCATED = NULL THEN MOVE 1 TO STATUS-CODE.

           END PROGRAM CBL_ALLOC_MEM.

       >> POP SOURCE FORMAT
