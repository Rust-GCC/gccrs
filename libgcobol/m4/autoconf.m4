#
# Minimal autoconf support for the COBOL language.
# Adapted from libga68/m4/autoconf.m4
#

# ------------------- #
# Language selection.
# ------------------- #

# AC_LANG(COBOL)
# ------------
AC_LANG_DEFINE([COBOL], [cbl], [COBOL], [COBC], [],
[ac_ext=cbl
ac_compile='$COBC -c $COBFLAGS conftest.$ac_ext >&AS_MESSAGE_LOG_FD'
ac_link='$COBC -o conftest$ac_exeext $COBFLAGS $LDFLAGS conftest.$ac_ext $LIBS >&AS_MESSAGE_LOG_FD'
ac_compiler_gnu=yes
])

#### # AC_LANG_COBOL
#### # -----------  There never was an obsolete AC_LANG_COBOL
#### AU_DEFUN([AC_LANG_COBOL], [AC_LANG(COBOL)])

# ------------------- #
# Producing programs.
# ------------------- #

# AC_LANG_PROGRAM(COBOL)([PROLOGUE], [BODY])
# ---------------------------------------------
m4_define([AC_LANG_PROGRAM(COBOL)],
[$1
$2])

# _AC_LANG_IO_PROGRAM(COBOL)
# -----------------------------
# Produce source that performs I/O.
m4_define([_AC_LANG_IO_PROGRAM(COBOL)],
[AC_LANG_PROGRAM([],
[       IDENTIFICATION DIVISION.
       PROGRAM-ID. iotest.
       ENVIRONMENT      DIVISION.
       INPUT-OUTPUT     SECTION.
       FILE-CONTROL.
       SELECT TEST-FILE ASSIGN "conftest.out".
       DATA             DIVISION.
       FILE             SECTION.
       FD  TEST-FILE.
       01  TEST-REC     PIC X(10).
       WORKING-STORAGE  SECTION.
       01  MSG          PIC X(10) VALUE 'Sam I am'.
       >>TURN EC-ALL CHECKING
       PROCEDURE        DIVISION.
           MOVE MSG to TEST-REC.
           OPEN  OUTPUT TEST-FILE.
           WRITE TEST-REC.
           CLOSE TEST-FILE.
         GOBACK.
])])

# AC_LANG_CALL(COBOL)(PROLOGUE, FUNCTION)
# ------------------------------------------
# Avoid conflicting decl of main.
m4_define([AC_LANG_CALL(COBOL)],
[AC_LANG_PROGRAM([],[])])

# AC_LANG_FUNC_LINK_TRY(COBOL)(FUNCTION)
# -----------------------------------------
# Try to link a program which calls FUNCTION.
m4_define([AC_LANG_FUNC_LINK_TRY(COBOL)],
[AC_LANG_PROGRAM([],[])])

#### # AC_LANG_BOOL_COMPILE_TRY(COBOL)(PROLOGUE, EXPRESSION)
#### # --------------------------------------------------------
#### # Return a program which is valid if EXPRESSION is nonzero.
#### m4_define([AC_LANG_BOOL_COMPILE_TRY(COBOL)],
#### [AC_LANG_PROGRAM([], [@<:@$2@:>@INT test multiple;
#### 		      0])])
#### 
#### # AC_LANG_INT_SAVE(COBOL)(PROLOGUE, EXPRESSION)
#### # ------------------------------------------------
#### m4_define([AC_LANG_INT_SAVE(COBOL)],
#### [AC_LANG_PROGRAM([
#### proc itoa = (int i) string:
####       begin if i = 0
#### 	    then "0"
#### 	    else int n := ABS i;
#### 		 string res;
#### 		 while n /= 0
#### 		 do int rem = n %* 10;
#### 		    res := (REPR (rem > 9 | (rem - 10) + ABS "a"
#### 				  | rem + ABS "0")
#### 			    + res);
#### 		    n %:= 10
#### 		 od;
#### 		 (i < 0 | "-" + res | res)
#### 	    fi
####       end;
#### 
#### int ret := 0;
#### int fd = fopen ("conftest.val", file o wronly)/
#### (fd = -1 | ret := 1; stop);
#### (fputs (fd, itoa ($2)) = 0 | ret := 1; stop)
#### ])])

# ---------------------- #
# Looking for compilers. #
# ---------------------- #

# AC_LANG_COMPILER(COBOL)
# --------------------------
AC_DEFUN([AC_LANG_COMPILER(COBOL)],
[AC_REQUIRE([AC_PROG_COBOL])])

# AC_PROG_COBOL
# ------------
AN_MAKEVAR([COBC], [AC_PROG_COBOL])
AN_PROGRAM([gcobol], [AC_PROG_COBOL])
AC_DEFUN([AC_PROG_COBOL],
[AC_LANG_PUSH(COBOL)dnl
AC_ARG_VAR([COBC], [COBOL compiler command])dnl
AC_ARG_VAR([COBFLAGS], [COBOL compiler flags])dnl
_AC_ARG_VAR_LDFLAGS()dnl
m4_ifval([$1],
      [AC_CHECK_TOOLS(COBOL, [$1])],
[AC_CHECK_TOOL(COBC, gcobol)
if test -z "$COBC"; then
  if test -n "$ac_tool_prefix"; then
    AC_CHECK_PROG(COBC, [${ac_tool_prefix}gcobol], [$ac_tool_prefix}gcobol])
  fi
fi
if test -z "$COBC"; then
  AC_CHECK_PROG(COBC, gcobol, gcobol, , , false)
fi
])

# Provide some information about the compiler.
_AS_ECHO_LOG([checking for _AC_LANG compiler version])
set X $ac_compile
ac_compiler=$[2]
_AC_DO_LIMIT([$ac_compiler --version >&AS_MESSAGE_LOG_FD])
m4_expand_once([_AC_COMPILER_EXEEXT])[]dnl
m4_expand_once([_AC_COMPILER_OBJEXT])[]dnl
COBFLAGS="-g -O2"
AC_LANG_POP(COBOL)dnl
])# AC_PROG_COBOL
