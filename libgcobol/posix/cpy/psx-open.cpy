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

      * >>DEFINE O_ACCMODE AS 3

        >>DEFINE O_APPEND      AS     1024
      * >>DEFINE O_ASYNC                   not used by open(2)
        >>DEFINE O_CLOEXEC     AS   524288
        >>DEFINE O_CREAT       AS       64
        >>DEFINE O_DIRECT      AS    16384 
        >>DEFINE O_DIRECTORY   AS    65536
        >>DEFINE O_DSYNC       AS     4096
        >>DEFINE O_EXCL        AS      128
      * >>DEFINE O_LARGEFILE   AS    32768 not POSIX
      * >>DEFINE O_NOATIME     AS   262144 not POSIX
        >>DEFINE O_NOCTTY      AS      256
        >>DEFINE O_NOFOLLOW    AS   131072
        >>DEFINE O_NONBLOCK    AS     2048
        >>DEFINE O_PATH        AS  2097152
        >>DEFINE O_RDONLY      AS        0
        >>DEFINE O_RDWR        AS        2
        >>DEFINE O_SYNC        AS  1052672
      * >>DEFINE O_TMPFILE     AS  4194304 + O_DIRECTORY  not POSIX
        >>DEFINE O_TRUNC       AS      512
        >>DEFINE O_WRONLY      AS        1


      * >>DEFINE S_IFBLK  AS 24576
      * >>DEFINE S_IFCHR  AS  8192
      * >>DEFINE S_IFDIR  AS 16384
      * >>DEFINE S_IFIFO  AS  4096
      * >>DEFINE S_IFLNK  AS 40960
      * >>DEFINE S_IFMT   AS 61440
      * >>DEFINE S_IFREG  AS 32768
      * >>DEFINE S_IFSOCK AS 49152
        
        >>DEFINE S_IRGRP  AS    32
        >>DEFINE S_IROTH  AS     4
        >>DEFINE S_IRUSR  AS   256
        >>DEFINE S_IRWXG  AS    56
        >>DEFINE S_IRWXO  AS     7
        >>DEFINE S_IRWXU  AS   448
        >>DEFINE S_ISGID  AS  1024
        >>DEFINE S_ISUID  AS  2048
        >>DEFINE S_ISVTX  AS   512
        >>DEFINE S_IWGRP  AS    16
        >>DEFINE S_IWOTH  AS     2
        >>DEFINE S_IWUSR  AS   128
        >>DEFINE S_IXGRP  AS     8
        >>DEFINE S_IXOTH  AS     1
        >>DEFINE S_IXUSR  AS    64

        >> POP source format

