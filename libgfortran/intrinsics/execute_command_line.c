/* Implementation of the EXECUTE_COMMAND_LINE intrinsic.
   Copyright (C) 2009-2019 Free Software Foundation, Inc.
   Contributed by François-Xavier Coudert.

This file is part of the GNU Fortran runtime library (libgfortran).

Libgfortran is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

Libgfortran is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#include "libgfortran.h"
#include <string.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef  HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#ifdef HAVE_POSIX_SPAWN
#include <spawn.h>
extern char **environ;
#endif
#if defined(HAVE_POSIX_SPAWN) || defined(HAVE_FORK)
#include <signal.h>
#endif

enum { EXEC_SYNCHRONOUS = -2, EXEC_NOERROR = 0, EXEC_SYSTEMFAILED,
       EXEC_CHILDFAILED, EXEC_INVALIDCOMMAND };
static const char *cmdmsg_values[] =
  { "",
    "Termination status of the command-language interpreter cannot be obtained",
    "Execution of child process impossible",
    "Invalid command line" };



static void
set_cmdstat (int *cmdstat, int value)
{
  if (cmdstat)
    *cmdstat = value;
  else if (value > EXEC_NOERROR)
    {
#define MSGLEN 200
      char msg[MSGLEN] = "EXECUTE_COMMAND_LINE: ";
      strncat (msg, cmdmsg_values[value], MSGLEN - strlen(msg) - 1);
      runtime_error ("%s", msg);
    }
}


#if defined(HAVE_WAITPID) && defined(HAVE_SIGACTION)
static void
sigchld_handler (int signum __attribute__((unused)))
{
  while (waitpid ((pid_t)(-1), NULL, WNOHANG) > 0) {}
}
#endif

static void
execute_command_line (const char *command, bool wait, int *exitstat,
		      int *cmdstat, char *cmdmsg,
		      gfc_charlen_type command_len,
		      gfc_charlen_type cmdmsg_len)
{
  /* Transform the Fortran string to a C string.  */
  char *cmd = fc_strdup (command, command_len);

  /* Flush all I/O units before executing the command.  */
  flush_all_units();

#if defined(HAVE_POSIX_SPAWN) || defined(HAVE_FORK)
  if (!wait)
    {
      /* Asynchronous execution.  */
      pid_t pid;

      set_cmdstat (cmdstat, EXEC_NOERROR);

#if defined(HAVE_SIGACTION) && defined(HAVE_WAITPID)
      static bool sig_init_saved;
      bool sig_init = __atomic_load_n (&sig_init_saved, __ATOMIC_RELAXED);
      if (!sig_init)
	{
	  struct sigaction sa;
	  sa.sa_handler = &sigchld_handler;
	  sigemptyset(&sa.sa_mask);
	  sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	  sigaction(SIGCHLD, &sa, 0);
	  __atomic_store_n (&sig_init_saved, true, __ATOMIC_RELAXED);
	}
#endif

#ifdef HAVE_POSIX_SPAWN
      const char * const argv[] = {"sh", "-c", cmd, NULL};
      if (posix_spawn (&pid, "/bin/sh", NULL, NULL,
		       (char * const* restrict) argv, environ))
	set_cmdstat (cmdstat, EXEC_CHILDFAILED);
#elif defined(HAVE_FORK)
      if ((pid = fork()) < 0)
        set_cmdstat (cmdstat, EXEC_CHILDFAILED);
      else if (pid == 0)
	{
	  /* Child process.  */
	  int res = system (cmd);
	  _exit (WIFEXITED(res) ? WEXITSTATUS(res) : res);
	}
#endif
    }
  else
#endif
    {
      /* Synchronous execution.  */
      int res = system (cmd);

      if (res == -1)
	set_cmdstat (cmdstat, EXEC_SYSTEMFAILED);
#if !defined(HAVE_POSIX_SPAWN) && !defined(HAVE_FORK)
      else if (!wait)
	set_cmdstat (cmdstat, EXEC_SYNCHRONOUS);
#endif
      else if (res == 127 || res == 126
#if defined(WEXITSTATUS) && defined(WIFEXITED)
	       || (WIFEXITED(res) && WEXITSTATUS(res) == 127)
	       || (WIFEXITED(res) && WEXITSTATUS(res) == 126)
#endif
	       )
	/* Shell return codes 126 and 127 mean that the command line could
	   not be executed for various reasons.  */
	set_cmdstat (cmdstat, EXEC_INVALIDCOMMAND);
      else
	set_cmdstat (cmdstat, EXEC_NOERROR);

      if (res != -1)
	{
#if defined(WEXITSTATUS) && defined(WIFEXITED)
	  *exitstat = WIFEXITED(res) ? WEXITSTATUS(res) : res;
#else
	  *exitstat = res;
#endif
	}
    }

  free (cmd);

  /* Now copy back to the Fortran string if needed.  */
  if (cmdstat && *cmdstat > EXEC_NOERROR && cmdmsg)
    fstrcpy (cmdmsg, cmdmsg_len, cmdmsg_values[*cmdstat],
		strlen (cmdmsg_values[*cmdstat]));
}


extern void
execute_command_line_i4 (const char *command, GFC_LOGICAL_4 *wait,
			 GFC_INTEGER_4 *exitstat, GFC_INTEGER_4 *cmdstat,
			 char *cmdmsg, gfc_charlen_type command_len,
			 gfc_charlen_type cmdmsg_len);
export_proto(execute_command_line_i4);

void
execute_command_line_i4 (const char *command, GFC_LOGICAL_4 *wait,
			 GFC_INTEGER_4 *exitstat, GFC_INTEGER_4 *cmdstat,
			 char *cmdmsg, gfc_charlen_type command_len,
			 gfc_charlen_type cmdmsg_len)
{
  bool w = wait ? *wait : true;
  int estat, estat_initial, cstat;

  if (exitstat)
    estat_initial = estat = *exitstat;

  execute_command_line (command, w, &estat, cmdstat ? &cstat : NULL,
			cmdmsg, command_len, cmdmsg_len);

  if (exitstat && estat != estat_initial)
    *exitstat = estat;
  if (cmdstat)
    *cmdstat = cstat;
}


extern void
execute_command_line_i8 (const char *command, GFC_LOGICAL_8 *wait,
			 GFC_INTEGER_8 *exitstat, GFC_INTEGER_8 *cmdstat,
			 char *cmdmsg, gfc_charlen_type command_len,
			 gfc_charlen_type cmdmsg_len);
export_proto(execute_command_line_i8);

void
execute_command_line_i8 (const char *command, GFC_LOGICAL_8 *wait,
			 GFC_INTEGER_8 *exitstat, GFC_INTEGER_8 *cmdstat,
			 char *cmdmsg, gfc_charlen_type command_len,
			 gfc_charlen_type cmdmsg_len)
{
  bool w = wait ? *wait : true;
  int estat, estat_initial, cstat;

  if (exitstat)
    estat_initial = estat = *exitstat;

  execute_command_line (command, w, &estat, cmdstat ? &cstat : NULL,
			cmdmsg, command_len, cmdmsg_len);

  if (exitstat && estat != estat_initial)
    *exitstat = estat;
  if (cmdstat)
    *cmdstat = cstat;
}
