/* Error and warning routines.
   Copyright (C) 2001-2023 J. Marcel van der Veer.
   Copyright (C) 2025 Jose E. Marchesi.

   Original implementation by J. Marcel van der Veer.
   Adapted and expanded for GCC by Jose E. Marchesi.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#define INCLUDE_MEMORY
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "diagnostic.h"

#include "a68.h"
#include "a68-pretty-print.h"

/*
 * Error handling routines.
 */

#define TABULATE(n) (8 * (n / 8 + 1) - n)

/* Severities handled by the DIAGNOSTIC function defined below.  */

#define A68_ERROR 0
#define A68_WARNING 1
#define A68_FATAL 2
#define A68_SCAN_ERROR 3
#define A68_INFORM 4

/* Give a diagnostic message.  */

ATTRIBUTE_A68_DIAG(6,0)
static bool
diagnostic (int sev, int opt,
	    NODE_T *p,
	    LINE_T *line,
	    char *pos,
	    const char *format, va_list args)
{
  int res = 0;

  /* Construct a diagnostic message.  */
  if (sev == A68_WARNING)
    WARNING_COUNT (&A68_JOB)++;
  else if (sev != A68_INFORM)
    ERROR_COUNT (&A68_JOB)++;

  /* Emit the corresponding GCC diagnostic at the proper location.  */
  location_t loc = UNKNOWN_LOCATION;

  if (p != NO_NODE)
    loc = a68_get_node_location (p);
  else if (line != NO_LINE)
    {
      if (pos == NO_TEXT)
	pos = STRING (line);
      loc = a68_get_line_location (line, pos);
    }

  /* Prepare rich location and diagnostics.  */
  rich_location rich_loc (line_table, loc);
  diagnostics::diagnostic_info diagnostic;
  enum diagnostics::kind kind;

  switch (sev)
    {
    case A68_FATAL:
      kind = diagnostics::kind::fatal;
      break;
    case A68_INFORM:
      kind = diagnostics::kind::note;
      break;
    case A68_WARNING:
      kind = diagnostics::kind::warning;
      break;
    case A68_SCAN_ERROR:
    case A68_ERROR:
      kind = diagnostics::kind::error;
      break;
    default:
      gcc_unreachable ();
    }

  va_list cargs;
  va_copy (cargs, args);
  diagnostic_set_info (&diagnostic, format, &cargs,
		       &rich_loc, kind);
  va_end (cargs);

  if (opt != 0)
    diagnostic.m_option_id = opt;
  res = diagnostic_report_diagnostic (global_dc, &diagnostic);

  if (sev == A68_SCAN_ERROR)
    exit (FATAL_EXIT_CODE);
  return res;
}

/* Give an intelligible error and exit.  A line is provided rather than a
   node so this can be used at scanning time.  */

void
a68_scan_error (LINE_T * u, char *v, const char *txt, ...)
{
  va_list args;

  va_start (args, txt);
  diagnostic (A68_SCAN_ERROR, 0, NO_NODE, u, v, txt, args);
  va_end (args);
}

/* Report a compilation error.  */

void
a68_error (NODE_T *p, const char *loc_str, ...)
{
  va_list args;

  va_start (args, loc_str);
  diagnostic (A68_ERROR, 0, p, NO_LINE, NO_TEXT, loc_str, args);
  va_end (args);
}

/* Report a compilation error in a node's pragmat.  */

void
a68_error_in_pragmat (NODE_T *p, size_t off,
		      const char *loc_str, ...)
{
  va_list args;

  LINE_T *line = NPRAGMAT_LINE (p);
  char *pos = NPRAGMAT_CHAR_IN_LINE (p) + off;

  va_start (args, loc_str);
  diagnostic (A68_ERROR, 0, NO_NODE, line, pos, loc_str, args);
  va_end (args);
  a68_inform (p, "pragmat applies to this construct");
}

/* Report a compilation warning.
   This function returns a boolean indicating whether a warning was
   emitted.  */

bool
a68_warning (NODE_T *p, int opt,
	     const char *loc_str, ...)
{
  bool res;
  va_list args;

  va_start (args, loc_str);
  res = diagnostic (A68_WARNING, opt, p, NO_LINE, NO_TEXT, loc_str, args);
  va_end (args);
  return res;
}

/* Report a compilation note.  */

void
a68_inform (NODE_T *p, const char *loc_str, ...)
{
  va_list args;

  va_start (args, loc_str);
  diagnostic (A68_INFORM, 0, p, NO_LINE, NO_TEXT, loc_str, args);
  va_end (args);
}
