------------------------------------------------------------------------------
--                                                                          --
--                 GNAT RUN-TIME LIBRARY (GNARL) COMPONENTS                 --
--                                                                          --
--                   A D A . I N T E R R U P T S . N A M E S                --
--                                                                          --
--                                  S p e c                                 --
--                                                                          --
--          Copyright (C) 1991-2023, Free Software Foundation, Inc.         --
--                                                                          --
-- GNARL is free software; you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 3,  or (at your option) any later ver- --
-- sion.  GNAT is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.                                     --
--                                                                          --
-- As a special exception under Section 7 of GPL version 3, you are granted --
-- additional permissions described in the GCC Runtime Library Exception,   --
-- version 3.1, as published by the Free Software Foundation.               --
--                                                                          --
-- You should have received a copy of the GNU General Public License and    --
-- a copy of the GCC Runtime Library Exception along with this program;     --
-- see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see    --
-- <http://www.gnu.org/licenses/>.                                          --
--                                                                          --
-- GNARL was developed by the GNARL team at Florida State University.       --
-- Extensive contributions were provided by Ada Core Technologies, Inc.     --
--                                                                          --
------------------------------------------------------------------------------

--  This is a Solaris version of this package

--  The following signals are reserved by the run time (native threads):

--  SIGFPE, SIGILL, SIGSEGV, SIGBUS, SIGTRAP, SIGABRT, SIGINT,
--  SIGLWP, SIGWAITING, SIGCANCEL, SIGSTOP, SIGKILL

--  The following signals are reserved by the run time (FSU threads):

--  SIGFPE, SIGILL, SIGSEGV, SIGBUS, SIGTRAP, SIGTERM, SIGABRT, SIGINT,
--  SIGLWP, SIGALRM, SIGVTALRM, SIGWAITING, SIGSTOP, SIGKILL

--  The pragma Unreserve_All_Interrupts affects the following signal(s):

--  SIGINT: made available for Ada handlers

with System.OS_Interface;

package Ada.Interrupts.Names is

   --  All identifiers in this unit are implementation defined

   pragma Implementation_Defined;

   --  Beware that the mapping of names to signals may be many-to-one. There
   --  may be aliases. Also, for all signal names that are not supported on the
   --  current system the value of the corresponding constant will be zero.

   SIGHUP : constant Interrupt_ID :=
     System.OS_Interface.SIGHUP;      --  hangup

   SIGINT : constant Interrupt_ID :=
     System.OS_Interface.SIGINT;      --  interrupt (rubout)

   SIGQUIT : constant Interrupt_ID :=
     System.OS_Interface.SIGQUIT;     --  quit (ASCD FS)

   SIGILL : constant Interrupt_ID :=
     System.OS_Interface.SIGILL;      --  illegal instruction (not reset)

   SIGTRAP : constant Interrupt_ID :=
     System.OS_Interface.SIGTRAP;     --  trace trap (not reset)

   SIGIOT : constant Interrupt_ID :=
     System.OS_Interface.SIGIOT;      --  IOT instruction

   SIGABRT : constant Interrupt_ID := --  used by abort,
     System.OS_Interface.SIGABRT;     --  replace SIGIOT in the  future

   SIGEMT : constant Interrupt_ID :=
     System.OS_Interface.SIGEMT;      --  EMT instruction

   SIGFPE : constant Interrupt_ID :=
     System.OS_Interface.SIGFPE;      --  floating point exception

   SIGKILL : constant Interrupt_ID :=
     System.OS_Interface.SIGKILL;     --  kill (cannot be caught or ignored)

   SIGBUS : constant Interrupt_ID :=
     System.OS_Interface.SIGBUS;      --  bus error

   SIGSEGV : constant Interrupt_ID :=
     System.OS_Interface.SIGSEGV;     --  segmentation violation

   SIGSYS : constant Interrupt_ID :=
     System.OS_Interface.SIGSYS;      --  bad argument to system call

   SIGPIPE : constant Interrupt_ID := --  write on a pipe with
     System.OS_Interface.SIGPIPE;     --  no one to read it

   SIGALRM : constant Interrupt_ID :=
     System.OS_Interface.SIGALRM;     --  alarm clock

   SIGTERM : constant Interrupt_ID :=
     System.OS_Interface.SIGTERM;     --  software termination signal from kill

   SIGUSR1 : constant Interrupt_ID :=
     System.OS_Interface.SIGUSR1;     --  user defined signal 1

   SIGUSR2 : constant Interrupt_ID :=
     System.OS_Interface.SIGUSR2;     --  user defined signal 2

   SIGCLD : constant Interrupt_ID :=
     System.OS_Interface.SIGCLD;      --  child status change

   SIGCHLD : constant Interrupt_ID :=
     System.OS_Interface.SIGCHLD;     --  4.3BSD's/POSIX name for SIGCLD

   SIGWINCH : constant Interrupt_ID :=
     System.OS_Interface.SIGWINCH;    --  window size change

   SIGURG : constant Interrupt_ID :=
     System.OS_Interface.SIGURG;      --  urgent condition on IO channel

   SIGPOLL : constant Interrupt_ID :=
     System.OS_Interface.SIGPOLL;     --  pollable event occurred

   SIGIO : constant Interrupt_ID :=   --  input/output possible,
     System.OS_Interface.SIGIO;       --  SIGPOLL alias (Solaris)

   SIGSTOP : constant Interrupt_ID :=
     System.OS_Interface.SIGSTOP;     --  stop (cannot be caught or ignored)

   SIGTSTP : constant Interrupt_ID :=
     System.OS_Interface.SIGTSTP;     --  user stop requested from tty

   SIGCONT : constant Interrupt_ID :=
     System.OS_Interface.SIGCONT;     --  stopped process has been continued

   SIGTTIN : constant Interrupt_ID :=
     System.OS_Interface.SIGTTIN;     --  background tty read attempted

   SIGTTOU : constant Interrupt_ID :=
     System.OS_Interface.SIGTTOU;     --  background tty write attempted

   SIGVTALRM : constant Interrupt_ID :=
     System.OS_Interface.SIGVTALRM;   --  virtual timer expired

   SIGPROF : constant Interrupt_ID :=
     System.OS_Interface.SIGPROF;     --  profiling timer expired

   SIGXCPU : constant Interrupt_ID :=
     System.OS_Interface.SIGXCPU;     --  CPU time limit exceeded

   SIGXFSZ : constant Interrupt_ID :=
     System.OS_Interface.SIGXFSZ;     --  filesize limit exceeded

   SIGPWR : constant Interrupt_ID :=
     System.OS_Interface.SIGPWR;      --  power-fail restart

   SIGWAITING : constant Interrupt_ID :=
     System.OS_Interface.SIGWAITING;  --  process's lwps blocked (Solaris)

   SIGLWP : constant Interrupt_ID :=
     System.OS_Interface.SIGLWP;      --  used by thread library (Solaris)

   SIGFREEZE : constant Interrupt_ID :=
     System.OS_Interface.SIGFREEZE;   --  used by CPR (Solaris)

--  what is CPR????

   SIGTHAW : constant Interrupt_ID :=
     System.OS_Interface.SIGTHAW;     --  used by CPR (Solaris)

   SIGCANCEL : constant Interrupt_ID :=
     System.OS_Interface.SIGCANCEL;     --  used for thread cancel (Solaris)

end Ada.Interrupts.Names;
