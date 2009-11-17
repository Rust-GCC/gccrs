#!/bin/sh

# Copyright 2009 The Go Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

# Create sysinfo.go.

# This shell script creates the sysinfo.go file which holds types and
# constants extracted from the system header files.  This relies on a
# hook in gcc: the -ggo option will generate debugging information in
# Go syntax.

# We currently #include all the files at once, which works, but leads
# to exposing some names which ideally should not be exposed, as they
# match grep patterns.  E.g., WCHAR_MIN gets exposed because it starts
# with W, like the wait flags.

CC=${CC:-gcc}
OUT=tmp-sysinfo.go

set -e

rm -f sysinfo.go

rm -f sysinfo.c
cat > sysinfo.c <<EOF
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <syscall.h>
#include <sys/epoll.h>
#include <sys/ptrace.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/user.h>
#include <unistd.h>
EOF

${CC} -D_GNU_SOURCE -ggo -S -o sysinfo.s sysinfo.c

echo 'package syscall' > ${OUT}

# Get all the consts, types, and funcs, skipping ones which could not
# be represented in Go and ones which we need to rewrite.  We also
# skip function declarations, as we don't need them here.  All the
# symbols will all have a leading underscore.
grep '^#GO' sysinfo.s | \
  grep -v '^#GO unknown' | \
  grep -v 'INVALID' | \
  grep -v '^#GO undef' | \
  grep -v '^#GO func' | \
  grep -v '^#GO type _timeval ' | \
  grep -v '^#GO type _timespec ' | \
  grep -v '^#GO type _epoll_' | \
  grep -v 'in6_addr' | \
  grep -v 'sockaddr_in6' | \
  sed -e 's/^#GO //' \
      -e 's/\([^a-zA-Z0-9_]\)_timeval\([^a-zA-Z0-9_]\)/\1Timeval\2/g' \
      -e 's/\([^a-zA-Z0-9_]\)_timespec\([^a-zA-Z0-9_]\)/\1Timespec\2/g' \
    >> ${OUT}

# The errno constants.
grep '^#GO const _E' sysinfo.s | \
  sed -e 's/#GO //' \
      -e 's/^\(const \)_\(E[^=]*\)\(.*\)$/\1\2 = _\2/' >> ${OUT}

# The O_xxx flags.
grep '^#GO const _\(O\|F\|FD\)_' sysinfo.s | \
  sed -e 's/#GO //' \
      -e 's/^\(const \)_\(\(O\|F\|FD\)_[^=]*\)\(.*\)$/\1\2 = _\2/' >> ${OUT}
if ! grep '^const O_CLOEXEC' ${OUT} >/dev/null 2>&1; then \
  echo "const O_CLOEXEC = 0" >> ${OUT}
fi

# The signal numbers.
grep '^#GO const _SIG[^_]' sysinfo.s | \
  grep -v '^#GO const _SIGEV_' | \
  sed -e 's/#GO //' \
      -e 's/^\(const \)_\(SIG[^=]*\)\(.*\)$/\1\2 = _\2/' >> ${OUT}

# The syscall numbers.  We force the names to upper case.
grep '^#GO const _SYS_' sysinfo.s | \
  sed -e 's/#GO //' \
      -e 's/^\(const \)_\(SYS_[^=]*\)\(.*\)$/\1\U\2\E = _\2/' >> ${OUT}

# Stat constants.
grep '^#GO const _S_' sysinfo.s | \
  sed -e 's/#GO //' \
      -e 's/^\(const \)_\(S_[^=]*\)\(.*\)$/\1\2 = _\2/' >> ${OUT}

# Process status constants.
grep '^#GO const _W' sysinfo.s |
  sed -e 's/#GO //' \
      -e 's/^\(const \)_\(W[^=]*\)\(.*\)$/\1\2 = _\2/' >> ${OUT}
if grep '^#GO const ___WALL = ' sysinfo.s >/dev/null 2>&1 \
   && ! grep '^#GO const _WALL = ' sysinfo.s >/dev/null 2>&1; then
  echo 'const WALL = ___WALL' >> ${OUT}
fi

# Networking constants.
grep '^#GO const _\(AF\|SOCK\|SOL\|SO\|IPPROTO\|TCP\)_' sysinfo.s |
  sed -e 's/#GO //' \
      -e 's/^\(const \)_\(\(AF\|SOCK\|SOL\|SO\|IPPROTO\|TCP\)_[^=]*\)\(.*\)$/\1\2 = _\2/' \
    >> ${OUT}
grep '^#GO const _SOMAXCONN' sysinfo.s |
  sed -e 's/#GO //' \
      -e 's/^\(const \)_\(SOMAXCONN[^=]*\)\(.*\)$/\1\2 = _\2/' \
    >> ${OUT}

# pathconf constants.
grep '^#GO const __PC' sysinfo.s |
  sed -e 's/#GO //' \
      -e 's/^\(const \)__\(PC[^=]*\)\(.*\)$/\1\2 = __\2/' >> ${OUT}

# The epoll constants were picked up by the errno constants, but we
# need to be sure the EPOLLRDHUP is defined.
if ! grep '^const EPOLLRDHUP' ${OUT} >/dev/null 2>&1; then \
  echo "const EPOLLRDHUP = 0x2000" >> ${OUT}
fi

# Ptrace constants.  We don't expose all the PTRACE flags, just the
# PTRACE_O_xxx and PTRACE_EVENT_xxx ones.
grep '^#GO const _PTRACE_O' sysinfo.s |
  sed -e 's/#GO //' \
      -e 's/^\(const \)_\(PTRACE_O[^=]*\)\(.*\)$/\1\2 = _\2/' >> ${OUT}
grep '^#GO const _PTRACE_EVENT' sysinfo.s |
  sed -e 's/#GO //' \
      -e 's/^\(const \)_\(PTRACE_EVENT[^=]*\)\(.*\)$/\1\2 = _\2/' >> ${OUT}
# We need PTRACE_SETOPTIONS and PTRACE_GETEVENTMSG, but they are not
# defined in older versions of glibc.
if ! grep '^const _PTRACE_SETOPTIONS' ${OUT} > /dev/null 2>&1; then
  echo "const _PTRACE_SETOPTIONS = 0x4200" >> ${OUT}
fi
if ! grep '^const PTRACE_O_TRACESYSGOOD' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_O_TRACESYSGOOD = 0x1" >> ${OUT}
fi
if ! grep '^const PTRACE_O_TRACEFORK' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_O_TRACEFORK = 0x2" >> ${OUT}
fi
if ! grep '^const PTRACE_O_TRACEVFORK' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_O_TRACEVFORK = 0x4" >> ${OUT}
fi
if ! grep '^const PTRACE_O_TRACECLONE' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_O_TRACECLONE = 0x8" >> ${OUT}
fi
if ! grep '^const PTRACE_O_TRACEEXEC' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_O_TRACEEXEC = 0x10" >> ${OUT}
fi
if ! grep '^const PTRACE_O_TRACEVFORKDONE' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_O_TRACEVFORKDONE = 0x20" >> ${OUT}
fi
if ! grep '^const PTRACE_O_TRACEEXIT' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_O_TRACEEXIT = 0x40" >> ${OUT}
fi
if ! grep '^const PTRACE_O_MASK' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_O_MASK = 0x7f" >> ${OUT}
fi
if ! grep '^const _PTRACE_GETEVENTMSG' ${OUT} > /dev/null 2>&1; then
  echo "const _PTRACE_GETEVENTMSG = 0x4201" >> ${OUT}
fi
if ! grep '^const PTRACE_EVENT_FORK' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_EVENT_FORK = 1" >> ${OUT}
fi
if ! grep '^const PTRACE_EVENT_VFORK' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_EVENT_VFORK = 2" >> ${OUT}
fi
if ! grep '^const PTRACE_EVENT_CLONE' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_EVENT_CLONE = 3" >> ${OUT}
fi
if ! grep '^const PTRACE_EVENT_EXEC' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_EVENT_EXEC = 4" >> ${OUT}
fi
if ! grep '^const PTRACE_EVENT_VFORK_DONE' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_EVENT_VFORK_DONE = 5" >> ${OUT}
fi
if ! grep '^const PTRACE_EVENT_EXIT' ${OUT} > /dev/null 2>&1; then
  echo "const PTRACE_EVENT_EXIT = 6" >> ${OUT}
fi

# The registers returned by PTRACE_GETREGS.  This is probably
# GNU/Linux specific.
regs=`grep '^#GO type _user_regs_struct struct' sysinfo.s | sed -e 's/#GO //'`
if test "$regs" != ""; then
  regs=`echo $regs | sed -e 's/type _user_regs_struct struct //'`
  regs=`echo $regs | sed -e 's/\([^a-zA-Z0-9_]*\)\([a-zA-Z0-9_]\)\([a-zA-Z0-9_]* [^;]*;\)/\1\U\2\E\3/g'`
  # FIXME: x86 specific.
  regs=`echo $regs | sed -e 's/X\([a-z]\)\([a-z]\) int32/ \U\1\E\2 uint16; X\1\2 uint16/g'`
  echo "type PtraceRegs struct $regs" >> ${OUT}
fi

# Some basic types.
echo 'type Size_t _size_t' >> ${OUT}
echo "type Ssize_t _ssize_t" >> ${OUT}
echo "type Off64_t _off64_t" >> ${OUT}
echo "type Mode_t _mode_t" >> ${OUT}
echo "type Pid_t _pid_t" >> ${OUT}
echo "type Uid_t _uid_t" >> ${OUT}
echo "type Gid_t _gid_t" >> ${OUT}
echo "type Socklen_t _socklen_t" >> ${OUT}

# The long type, needed because that is the type that ptrace returns.
sizeof_long=`grep '^#GO const ___SIZEOF_LONG__ = ' sysinfo.s | sed -e 's/.*= //'`
if test "$sizeof_long" = "4"; then
  echo "type _C_long int32" >> ${OUT}
elif test "$sizeof_long" = "8"; then
  echo "type _C_long int64" >> ${OUT}
else
  echo 1>&2 "mksysinfo.sh: could not determine size of long (got $sizeof_long)"
  exit 1
fi

# The time structures need special handling: we need to name the
# types, so that we can cast integers to the right types when
# assigning to the structures.
timeval=`grep '#GO type _timeval ' sysinfo.s`
timeval_sec=`echo $timeval | sed -n -e 's/^.*tv_sec \([^ ]*\);.*$/\1/p'`
timeval_usec=`echo $timeval | sed -n -e 's/^.*tv_usec \([^ ]*\);.*$/\1/p'`
echo "type Timeval_sec_t $timeval_sec" >> ${OUT}
echo "type Timeval_usec_t $timeval_usec" >> ${OUT}
echo $timeval | \
  sed -e 's/^#GO type _timeval /type Timeval /' \
      -e 's/tv_sec *[a-zA-Z0-9_]*/Sec Timeval_sec_t/' \
      -e 's/tv_usec *[a-zA-Z0-9_]*/Usec Timeval_usec_t/' >> ${OUT}
timespec=`grep '#GO type _timespec ' sysinfo.s`
timespec_sec=`echo $timespec | sed -n -e 's/^.*tv_sec \([^ ]*\);.*$/\1/p'`
timespec_nsec=`echo $timespec | sed -n -e 's/^.*tv_nsec \([^ ]*\);.*$/\1/p'`
echo "type Timespec_sec_t $timespec_sec" >> ${OUT}
echo "type Timespec_nsec_t $timespec_nsec" >> ${OUT}
echo $timespec | \
  sed -e 's/^#GO type _timespec /type Timespec /' \
      -e 's/tv_sec *[a-zA-Z0-9_]*/Sec Timespec_sec_t/' \
      -e 's/tv_nsec *[a-zA-Z0-9_]*/Nsec Timespec_nsec_t/' >> ${OUT}

# The stat type.
grep '#GO type _stat ' sysinfo.s | \
  sed -e 's/#GO //' \
      -e 's/type _stat/type Stat_t/' \
      -e 's/st_dev/Dev/' \
      -e 's/st_ino/Ino/' \
      -e 's/st_nlink/Nlink/' \
      -e 's/st_mode/Mode/' \
      -e 's/st_uid/Uid/' \
      -e 's/st_gid/Gid/' \
      -e 's/st_rdev/Rdev/' \
      -e 's/st_size/Size/' \
      -e 's/st_blksize/Blksize/' \
      -e 's/st_blocks/Blocks/' \
      -e 's/st_atim/Atime/' \
      -e 's/st_mtim/Mtime/' \
      -e 's/st_ctim/Ctime/' \
      -e 's/\([^a-zA-Z0-9_]\)_timeval\([^a-zA-Z0-9_]\)/\1Timeval\2/g' \
      -e 's/\([^a-zA-Z0-9_]\)_timespec\([^a-zA-Z0-9_]\)/\1Timespec\2/g' \
    >> ${OUT}

# The directory searching types.
grep '#GO type _dirent ' sysinfo.s | \
  sed -e 's/#GO //' \
      -e 's/type _dirent/type Dirent/' \
      -e 's/d_name/Name/' \
      -e 's/]int8/]byte/' \
      -e 's/d_ino/Ino/' \
      -e 's/d_off/Off/' \
      -e 's/d_reclen/Reclen/' \
      -e 's/d_type/Type/' \
    >> ${OUT}
echo "type DIR _DIR" >> ${OUT}

# The rusage struct.
grep '#GO type _rusage ' sysinfo.s | \
  sed -e 's/#GO //' \
      -e 's/type _rusage/type Rusage/' \
      -e 's/ru_\([a-z]\)/\U\1/g' \
      -e 's/\([^a-zA-Z0-9_]\)_timeval\([^a-zA-Z0-9_]\)/\1Timeval\2/g' \
      -e 's/\([^a-zA-Z0-9_]\)_timespec\([^a-zA-Z0-9_]\)/\1Timespec\2/g' \
    >> ${OUT}

mv -f ${OUT} sysinfo.go
exit $?
