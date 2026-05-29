       >>PUSH SOURCE FORMAT
       >>SOURCE FIXED
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

        Copy "posix-open.cpy".
      * int open(const char *pathname, int flags);
        Identification Division.
        Function-ID. posix-open.

        Environment Division.
        Configuration Section.
          Source-Computer. Sesame-Cookie
        >>IF DEBUGGING-MODE is Defined
          With Debugging Mode
        >>END-IF
          .

        Data Division.
        Working-Storage Section.
          77 Ws-pathname PIC X(8192).
          77 Ws-mode-ptr Pointer.
          77 Ws-mode     PIC 9(8) COMP-5 VALUE 0.
        Linkage Section.
          77 Return-Value Binary-Long Signed.
          01 Lk-pathname PIC X ANY LENGTH.
          01 Lk-flags    PIC 9(8) comp-5.
          01 Lk-mode     PIC 9(8) comp-5.

        Procedure Division using
             By Reference Lk-pathname,
             By Value Lk-flags,
             By Value Lk-mode
             Returning Return-Value.

          Move Lk-pathname To Ws-pathname.
          Inspect Ws-pathname
                  Replacing Trailing Space By Low-Value

      D   Display 'posix-open: Ws-pathname ', Ws-pathname.
      D   Perform Show-Flags.

          Set ws-mode-ptr to Address Of Lk-mode.

          If ws-mode-ptr NOT = NULL Then *> O_CREAT requires mode
            Move Lk-mode to Ws-mode.

          Call "posix_open" using Ws-pathname,
                              By Value Lk-flags,
                              By Value Ws-mode,
                              Returning Return-Value.
          Goback.

          Show-Flags Section.

            Display 'Flags: ', Function Hex-Of(Lk-flags).

        End Function posix-open.
        >> POP SOURCE FORMAT
