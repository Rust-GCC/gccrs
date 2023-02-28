/* wraptime.c provides access to time related system calls.

Copyright (C) 2009-2022 Free Software Foundation, Inc.
Contributed by Gaius Mulley <gaius.mulley@southwales.ac.uk>.

This file is part of GNU Modula-2.

GNU Modula-2 is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GNU Modula-2 is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#include "config.h"

#if defined(HAVE_SYS_TYPES_H)
#include "sys/types.h"
#endif

#if defined(HAVE_SYS_TIME_H)
#include "sys/time.h"
#endif

#if defined(HAVE_TIME_H)
#include "time.h"
#endif

#if defined(HAVE_MALLOC_H)
#include "malloc.h"
#endif

#if defined(HAVE_LIMITS_H)
#include "limits.h"
#endif

#if !defined(TRUE)
#define TRUE (1 == 1)
#endif

#if !defined(FALSE)
#define FALSE (1 == 0)
#endif

#if !defined(NULL)
#define NULL (void *)0
#endif

/* InitTimeval returns a newly created opaque type.  */

#if defined(HAVE_TIMEVAL) && defined(HAVE_MALLOC_H)
struct timeval *
wraptime_InitTimeval (void)
{
  return (struct timeval *)malloc (sizeof (struct timeval));
}
#else
void *
wraptime_InitTimeval (void)
{
  return NULL;
}
#endif

/* KillTimeval deallocates the memory associated with an opaque type.  */

struct timeval *
wraptime_KillTimeval (void *tv)
{
#if defined(HAVE_MALLOC_H)
  free (tv);
#endif
  return NULL;
}

/* InitTimezone returns a newly created opaque type.  */

#if defined(HAVE_STRUCT_TIMEZONE) && defined(HAVE_MALLOC_H)
struct timezone *
wraptime_InitTimezone (void)
{
  return (struct timezone *)malloc (sizeof (struct timezone));
}
#else
void *
wraptime_InitTimezone (void)
{
  return NULL;
}
#endif

/* KillTimezone - deallocates the memory associated with an opaque
   type.  */

struct timezone *
wraptime_KillTimezone (struct timezone *tv)
{
#if defined(HAVE_MALLOC_H)
  free (tv);
#endif
  return NULL;
}

/* InitTM - returns a newly created opaque type.  */

#if defined(HAVE_STRUCT_TM) && defined(HAVE_MALLOC_H)
struct tm *
wraptime_InitTM (void)
{
  return (struct tm *)malloc (sizeof (struct tm));
}
#else
void *
wraptime_InitTM (void)
{
  return NULL;
}
#endif

/* KillTM - deallocates the memory associated with an opaque type.  */

struct tm *
wraptime_KillTM (struct tm *tv)
{
#if defined(HAVE_MALLOC_H)
  free (tv);
#endif
  return NULL;
}

/* gettimeofday - calls gettimeofday(2) with the same parameters, tv,
   and, tz.  It returns 0 on success.  */

#if defined(HAVE_STRUCT_TIMEZONE) && defined(HAVE_GETTIMEOFDAY)
int
wraptime_gettimeofday (void *tv, struct timezone *tz)
{
  return gettimeofday (tv, tz);
}
#else
int
wraptime_gettimeofday (void *tv, void *tz)
{
  return -1;
}
#endif

/* settimeofday - calls settimeofday(2) with the same parameters, tv,
   and, tz.  It returns 0 on success.  */

#if defined(HAVE_STRUCT_TIMEZONE) && defined(HAVE_SETTIMEOFDAY)
int
wraptime_settimeofday (void *tv, struct timezone *tz)
{
  return settimeofday (tv, tz);
}
#else
int
wraptime_settimeofday (void *tv, void *tz)
{
  return -1;
}
#endif

/* wraptime_GetFractions - returns the tv_usec field inside the
   timeval structure.  */

#if defined(HAVE_TIMEVAL)
unsigned int
wraptime_GetFractions (struct timeval *tv)
{
  return (unsigned int)tv->tv_usec;
}
#else
unsigned int
wraptime_GetFractions (void *tv)
{
  return (unsigned int)-1;
}
#endif

/* localtime_r - returns the tm parameter, m, after it has been
   assigned with appropriate contents determined by, tv.  Notice that
   this procedure function expects, timeval, as its first parameter
   and not a time_t (as expected by the posix equivalent).  */

#if defined(HAVE_TIMEVAL)
struct tm *
wraptime_localtime_r (struct timeval *tv, struct tm *m)
{
  return localtime_r (&tv->tv_sec, m);
}
#else
struct tm *
wraptime_localtime_r (void *tv, struct tm *m)
{
  return m;
}
#endif

/* wraptime_GetYear - returns the year from the structure, m.  */

#if defined(HAVE_STRUCT_TM)
unsigned int
wraptime_GetYear (struct tm *m)
{
  return m->tm_year;
}
#else
unsigned int
wraptime_GetYear (void *m)
{
  return (unsigned int)-1;
}
#endif

/* wraptime_GetMonth - returns the month from the structure, m.  */

#if defined(HAVE_STRUCT_TM)
unsigned int
wraptime_GetMonth (struct tm *m)
{
  return m->tm_mon;
}
#else
unsigned int
wraptime_GetMonth (void *m)
{
  return (unsigned int)-1;
}
#endif

/* wraptime_GetDay - returns the day of the month from the structure,
   m.  */

#if defined(HAVE_STRUCT_TM)
unsigned int
wraptime_GetDay (struct tm *m)
{
  return m->tm_mday;
}
#else
unsigned int
wraptime_GetDay (void *m)
{
  return (unsigned int)-1;
}
#endif

/* wraptime_GetHour - returns the hour of the day from the structure,
   m.  */

#if defined(HAVE_STRUCT_TM)
unsigned int
wraptime_GetHour (struct tm *m)
{
  return m->tm_hour;
}
#else
unsigned int
wraptime_GetHour (void *m)
{
  return (unsigned int)-1;
}
#endif

/* wraptime_GetMinute - returns the minute within the hour from the
   structure, m.  */

#if defined(HAVE_STRUCT_TM)
unsigned int
wraptime_GetMinute (struct tm *m)
{
  return m->tm_min;
}
#else
unsigned int
wraptime_GetMinute (void *m)
{
  return (unsigned int)-1;
}
#endif

/* wraptime_GetSecond - returns the seconds in the minute from the
   structure, m.  The return value will always be in the range 0..59.
   A leap minute of value 60 will be truncated to 59.  */

#if defined(HAVE_STRUCT_TM)
unsigned int
wraptime_GetSecond (struct tm *m)
{
  if (m->tm_sec == 60)
    return 59;
  else
    return m->tm_sec;
}
#else
unsigned int
wraptime_GetSecond (void *m)
{
  return (unsigned int)-1;
}
#endif

/* wraptime_GetSummerTime - returns true if summer time is in effect.  */

#if defined(HAVE_STRUCT_TIMEZONE)
unsigned int
wraptime_GetSummerTime (struct timezone *tz)
{
  return tz->tz_dsttime != 0;
}
#else
unsigned int
wraptime_GetSummerTime (void *tz)
{
  return FALSE;
}
#endif

/* wraptime_GetDST - returns the number of minutes west of GMT.  */

#if defined(HAVE_STRUCT_TIMEZONE)
int
wraptime_GetDST (struct timezone *tz)
{
  return tz->tz_minuteswest;
}
#else
int
wraptime_GetDST (void *tz)
{
#if defined(INT_MIN)
  return INT_MIN;
#else
  return (int)((unsigned int)-1);
#endif
}
#endif

/* SetTimezone - set the timezone field inside timeval, tv.  */

#if defined(HAVE_STRUCT_TIMEZONE)
void
wraptime_SetTimezone (struct timezone *tz, int zone, int minuteswest)
{
  tz->tz_dsttime = zone;
  tz->tz_minuteswest = minuteswest;
}
#else
void
wraptime_SetTimezone (void *tz, int zone, int minuteswest)
{
}
#endif

/* SetTimeval - sets the fields in tm, t, with: second, minute, hour,
   day, month, year, fractions.  */

#if defined(HAVE_TIMEVAL)
void
wraptime_SetTimeval (struct tm *t, unsigned int second, unsigned int minute,
                     unsigned int hour, unsigned int day, unsigned int month,
                     unsigned int year, unsigned int yday, unsigned int wday,
                     unsigned int isdst)
{
  t->tm_sec = second;
  t->tm_min = minute;
  t->tm_hour = hour;
  t->tm_mday = day;
  t->tm_mon = month;
  t->tm_year = year;
  t->tm_yday = yday;
  t->tm_wday = wday;
  t->tm_isdst = isdst;
}
#else
void
wraptime_SetTimeval (void *t, unsigned int second, unsigned int minute,
                     unsigned int hour, unsigned int day, unsigned int month,
                     unsigned int year, unsigned int yday, unsigned int wday,
                     unsigned int isdst)
{
}
#endif

/* init - init/finish functions for the module */

void
_M2_wraptime_init ()
{
}
void
_M2_wraptime_fini ()
{
}
