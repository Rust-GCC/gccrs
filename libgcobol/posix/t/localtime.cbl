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

      * Include the posix-stat and posix-localtime functions.
        COPY posix-stat.
        COPY posix-localtime.
        COPY posix-errno.

        Identification Division.
        Program-ID. test-localtime.
        Data Division.
        Working-Storage Section.
        77 Return-Value Usage Binary-Long.
        77 Stat-Status  Usage Binary-Long Value 1.
        77 Filename     Pic x(80) Value 'Makefile'.
        77 Msg Pic x(100).
        01 Lk-statbuf.
        COPY statbuf.
        01 Lk-tm.
        COPY tm.
        01 Today.
           02 tm_year PIC 9999.
           02 tm_mon  PIC 99.
           02 tm_wday PIC 99.

        Procedure Division.
        Display 'calling posix-stat for ' Function Trim(Filename) ' ...'
        Move Function posix-stat(Filename, lk-statbuf) to Return-Value.
        Display 'posix-stat returned: ' Return-Value.
        If Return-Value < 0 then
          Display Function Trim(Filename) ': '
                  'errno ', Function posix-errno(Msg), ': ' Msg
          Goback.

        Display 'calling posix-localtime ...'
        Move Function posix-localtime(st_mtime, lk-tm) to Return-Value.
        Display 'posix-localtime returned: ' Return-Value.
        If Return-Value < 0 then
          Display 'posix-localtime: ', Function Trim(Filename) ': '
                  'errno ', Function posix-errno(Msg), ': ' Msg
                ' (st_mtime ' st_mtime ')'
          Goback.
        Move Corresponding Lk-tm to Today.
        Add 1900 to tm_year of Today.
        Display "'" Function trim(Filename) "'"
                ' (st_mtime ' st_mtime ') modified '
                tm_year of Today '-'
                tm_mon  of Today '-'
                tm_wday of Today.
        Goback.
        >> POP SOURCE FORMAT