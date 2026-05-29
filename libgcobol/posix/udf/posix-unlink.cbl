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

        Copy "posix-unlink.cpy".
        Identification Division.
        Function-ID. posix-unlink.
        Data Division.
        Working-Storage Section.
          77 bufsize Usage Binary-Long.
          77 Ws-pathname PIC X(8192).
        Linkage Section.
          77 Return-Value Binary-Long.
          01 Lk-pathname PIC X ANY LENGTH.

        Procedure Division using
             By Reference Lk-pathname,
             Returning Return-Value.

          Move Lk-pathname To Ws-pathname.
      D   Inspect Ws-pathname
      D           Replacing Trailing Space By Low-Value

          Inspect Backward Ws-pathname Replacing Leading Space,
                                                      By Low-Value.
          Call "unlink" using
             By Reference Ws-pathname,
             Returning Return-Value.
          Goback.
        End Function posix-unlink.
        >> POP SOURCE FORMAT
