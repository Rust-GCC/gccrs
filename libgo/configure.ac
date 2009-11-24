# configure.ac -- Go library configure script.

# Copyright 2009 The Go Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

# Process this file with autoreconf to produce configure.

AC_PREREQ(2.64)
AC_INIT(package-unused, version-unused,, libgo)
AC_CONFIG_SRCDIR(Makefile.am)
AC_CONFIG_HEADER(config.h)

libtool_VERSION=1:0:0
AC_SUBST(libtool_VERSION)

AM_ENABLE_MULTILIB(, ..)

AC_CANONICAL_SYSTEM
target_alias=${target_alias-$host_alias}

AM_INIT_AUTOMAKE([1.9.3 no-define foreign -Wall])
AH_TEMPLATE(PACKAGE, [Name of package])
AH_TEMPLATE(VERSION, [Version number of package])

m4_rename([_AC_ARG_VAR_PRECIOUS],[glibgo_PRECIOUS])
m4_define([_AC_ARG_VAR_PRECIOUS],[])
AC_PROG_CC
AC_PROG_GO
m4_rename_force([glibgo_PRECIOUS],[_AC_ARG_VAR_PRECIOUS])

AC_SUBST(CFLAGS)

AM_MAINTAINER_MODE

AC_PROG_RANLIB

AC_LIBTOOL_DLOPEN
AM_PROG_LIBTOOL
AC_SUBST(enable_shared)
AC_SUBST(enable_static)

WARN_FLAGS='-Wall -Wextra -Wwrite-strings -Wcast-qual'
AC_SUBST(WARN_FLAGS)

dnl FIXME: This should be controlled by --enable-maintainer-mode.
WERROR="-Werror"
AC_SUBST(WERROR)

glibgo_toolexecdir=no
glibgo_toolexeclibdir=no
glibgo_prefixdir=$prefix

AC_MSG_CHECKING([for --enable-version-specific-runtime-libs])
AC_ARG_ENABLE([version-specific-runtime-libs],
  AC_HELP_STRING([--enable-version-specific-runtime-libs],
                 [Specify that runtime libraries should be installed in a compiler-specific directory]),
  [case "$enableval" in
    yes) version_specific_libs=yes ;;
    no)  version_specific_libs=no ;;
    *)   AC_MSG_ERROR([Unknown argument to enable/disable version-specific libs]);;
   esac],
  [version_specific_libs=no])
AC_MSG_RESULT($version_specific_libs)

# Version-specific runtime libs processing.
if test $version_specific_libs = yes; then
  glibgo_toolexecdir='${libdir}/gcc/${host_alias}'
  glibgo_toolexeclibdir='${toolexecdir}/${gcc_version}$(MULTISUBDIR)'
fi

# Calculate glibgo_toolexecdir, glibgo_toolexeclibdir
# Install a library built with a cross compiler in tooldir, not libdir.
if test x"$glibgo_toolexecdir" = x"no"; then
  if test -n "$with_cross_host" &&
     test x"$with_cross_host" != x"no"; then
    glibgo_toolexecdir='${exec_prefix}/${host_alias}'
    glibgo_toolexeclibdir='${toolexecdir}/lib'
  else
    glibgo_toolexecdir='${libdir}/gcc/${host_alias}'
    glibgo_toolexeclibdir='${libdir}'
  fi
  multi_os_directory=`$CC -print-multi-os-directory`
  case $multi_os_directory in
    .) ;; # Avoid trailing /.
    *) glibgo_toolexeclibdir=$glibgo_toolexeclibdir/$multi_os_directory ;;
  esac
fi

AC_SUBST(glibgo_prefixdir)
AC_SUBST(glibgo_toolexecdir)
AC_SUBST(glibgo_toolexeclibdir)

# See if the user wants to configure without libffi.  Some
# architectures don't support it.  FIXME: We should set a default
# based on the host.
AC_ARG_WITH(libffi,
  AS_HELP_STRING([--without-libffi],
                 [don't use libffi]),
  [:],
  [with_libffi=${with_libffi_default-yes}])

LIBFFI=
LIBFFIINCS=
if test "$with_libffi" != no; then
   AC_DEFINE(USE_LIBFFI, 1, [Define if we're to use libffi.])
   LIBFFI=../libffi/libffi_convenience.la
   LIBFFIINCS='-I$(top_srcdir)/../libffi/include -I../libffi/include'
fi
AC_SUBST(LIBFFI)
AC_SUBST(LIBFFIINCS)

is_darwin=no
is_linux=no
case ${host} in
  *-*-darwin*) is_darwin=yes ;;
  *-*-linux*)  is_linux=yes  ;;
esac
AM_CONDITIONAL(LIBGO_IS_DARWIN, test $is_darwin = yes)
AM_CONDITIONAL(LIBGO_IS_LINUX, test $is_linux = yes)

is_386=no
is_x86_64=no
is_arm=no
case ${host} in
changequote(,)dnl
  i[34567]86-*-*)
changequote([,])dnl
    is_386=yes
    ;;
  x86_64-*-*)
    if test "$with_multisubdir" = "32"; then
      is_386=yes
    else
      is_x86_64=yes
    fi
    ;;
  arm*-*-* | strongarm*-*-* | ep9312*-*-* | xscale-*-*)
    is_arm=yes
    ;;
esac
AM_CONDITIONAL(LIBGO_IS_386, test $is_386 = yes)
AM_CONDITIONAL(LIBGO_IS_X86_64, test $is_x86_64 = yes)
AM_CONDITIONAL(LIBGO_IS_ARM, test $is_arm = yes)

dnl Use -fsplit-stack when compiling C code if available.
AC_CACHE_CHECK([whether -fsplit-stack is supported],
[ac_cv_libgo_split_stack_supported],
[CFLAGS_hold=$CFLAGS
CFLAGS="$CFLAGS -fsplit-stack"
AC_COMPILE_IFELSE([[int i;]],
[ac_cv_libgo_split_stack_supported=yes],
[ac_cv_libgo_split_stack_supported=no])
CFLAGS=$CFLAGS_hold])
if test "$ac_cv_libgo_split_stack_supported" = yes; then
  SPLIT_STACK=-fsplit-stack
else
  SPLIT_STACK=
fi
AC_SUBST(SPLIT_STACK)

AC_C_BIGENDIAN

AC_CACHE_SAVE

if test ${multilib} = yes; then
  multilib_arg="--enable-multilib"
else
  multilib_arg=
fi

AC_CONFIG_FILES(Makefile)

AC_CONFIG_COMMANDS([default],
[if test -n "$CONFIG_FILES"; then
   # Multilibs need MULTISUBDIR defined correctly in certain makefiles so
   # that multilib installs will end up installed in the correct place.
   # The testsuite needs it for multilib-aware ABI baseline files.
   # To work around this not being passed down from config-ml.in ->
   # srcdir/Makefile.am -> srcdir/{src,libsupc++,...}/Makefile.am, manually
   # append it here.  Only modify Makefiles that have just been created.
   #
   # Also, get rid of this simulated-VPATH thing that automake does.
   cat > vpsed << \_EOF
s!`test -f '$<' || echo '$(srcdir)/'`!!
_EOF
   for i in $SUBDIRS; do
    case $CONFIG_FILES in
     *${i}/Makefile*)
       #echo "Adding MULTISUBDIR to $i/Makefile"
       sed -f vpsed $i/Makefile > tmp
       grep '^MULTISUBDIR =' Makefile >> tmp
       mv tmp $i/Makefile
       ;;
    esac
   done
   rm vpsed
 fi
],
[
# Variables needed in config.status (file generation) which aren't already
# passed by autoconf.
SUBDIRS="$SUBDIRS"
])

AC_OUTPUT
