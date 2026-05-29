        >> PUSH source format
        >>SOURCE format is fixed

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

        COPY posix-mkdir.
        COPY posix-errno.

        Identification Division.
        Program-ID. test-errno.
        Data Division.
        Working-Storage Section.
        77 Return-Value Binary-Long.
        77 Exit-Status Binary-Long Value 1.
        77 error-msg PIC X(100).
        77 errnum Binary-Long.
        77 Filename PIC X(100) Value '/'.

        Procedure Division.
        Display 'calling posix-mkdir with a foolish name ...'
        Move Function posix-mkdir(Filename, 0) to Return-Value.
        If Return-Value <> 0
            Display 'calling posix-errno ...'
            Move Function posix-errno(error-msg) to errnum
            Display 'error: "' Filename '": ' error-msg ' (' errnum ')'
            Goback with Error Status errnum
        Else
            Display 'Return-Value is ' Return-Value
        End-If.

        Goback.
        >> POP SOURCE FORMAT
