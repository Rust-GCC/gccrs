(* Environment.mod provides access to the environment settings of a process.

Copyright (C) 2001-2023 Free Software Foundation, Inc.
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
<http://www.gnu.org/licenses/>.  *)

IMPLEMENTATION MODULE Environment ;


FROM SYSTEM IMPORT ADR ;
FROM libc IMPORT getenv, putenv ;
FROM ASCII IMPORT nul ;
FROM StrLib IMPORT StrCopy ;


(*
   GetEnvironment - gets the environment variable Env and places
      	       	    a copy of its value into string, dest.
                    It returns TRUE if the string Env was found in
                    the processes environment.
*)

PROCEDURE GetEnvironment (Env: ARRAY OF CHAR;
                          VAR dest: ARRAY OF CHAR) : BOOLEAN ;
VAR
   High,
   i   : CARDINAL ;
   Addr: POINTER TO CHAR ;
BEGIN
   i := 0 ;
   High := HIGH (dest) ;
   Addr := getenv (ADR (Env)) ;
   WHILE (i<High) AND (Addr#NIL) AND (Addr^#nul) DO
      dest[i] := Addr^ ;
      INC (Addr) ;
      INC (i)
   END ;
   IF i<High
   THEN
      dest[i] := nul
   END ;
   RETURN Addr#NIL
END GetEnvironment ;


(*
   PutEnvironment - change or add an environment variable definition EnvDef.
                    TRUE is returned if the environment variable was
                    set or changed successfully.
*)

PROCEDURE PutEnvironment (EnvDef: ARRAY OF CHAR) : BOOLEAN ;
BEGIN
   RETURN putenv (ADR (EnvDef)) = 0
END PutEnvironment ;


END Environment.
