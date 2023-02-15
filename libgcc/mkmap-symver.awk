# Generate an ELF symbol version map a-la Solaris and GNU ld.
#	Copyright (C) 2007-2023 Free Software Foundation, Inc.
#	Contributed by Richard Henderson <rth@cygnus.com>
#
# This file is part of GCC.
#
# GCC is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 3, or (at your option) any later
# version.
#
# GCC is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
# License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GCC; see the file COPYING3.  If not see
# <http://www.gnu.org/licenses/>.

BEGIN {
  state = "nm";
  sawsymbol = 0;
  if (leading_underscore)
    prefix = "_";
  else
    prefix = "";
}

# Remove comment and blank lines.
/^ *#/ || /^ *$/ {
  next;
}

# We begin with nm input.  Collect the set of symbols that are present
# so that we cannot emit them into the final version script -- Solaris
# complains at us if we do.

state == "nm" && /^%%/ {
  state = "ver";
  next;
}

state == "nm" && ($1 == "U" || $2 == "U") {
  next;
}

state == "nm" && NF == 3 {
  split ($3, s, "@")
  if (skip_underscore && substr(s[1], 1, 1) == "_")
      symname = substr(s[1], 2);
  else
      symname = s[1];
  def[symname] = 1;
  sawsymbol = 1;
  next;
}

state == "nm" {
  next;
}

# Now we process a simplified variant of the Solaris symbol version
# script.  We have one symbol per line, no semicolons, simple markers
# for beginning and ending each section, and %inherit markers for
# describing version inheritance.  A symbol may appear in more than
# one symbol version, and the last seen takes effect.
# The magic version name '%exclude' causes all the symbols given that
# version to be dropped from the output (unless a later version overrides).

NF == 3 && $1 == "%inherit" {
  inherit[$2] = $3;
  next;
}

NF == 2 && $2 == "{" {
  if ($1 != "%exclude")
    libs[$1] = 1;
  thislib = $1;
  next;
}

$1 == "}" {
  thislib = "";
  next;
}

{
  sym = prefix $1;
  symbols[sym] = 1
  if (thislib != "%exclude")
    ver[sym, thislib] = 1;
  else {
    for (l in libs)
      ver[sym, l] = 0;
  }
  next;
}

END {
  if (!sawsymbol)
    {
      print "No symbols seen -- broken or mis-installed nm?" | "cat 1>&2";
      exit 1;
    }
  for (l in libs)
    output(l);
}

function output(lib) {
  if (done[lib])
    return;
  done[lib] = 1;
  if (inherit[lib])
    output(inherit[lib]);

  empty=1
  for (sym in symbols)
    if ((ver[sym, lib] != 0) && (sym in def))
      {
	if (empty)
	  {
	    printf("%s {\n", lib);
	    printf("  global:\n");
	    empty = 0;
	  }
	printf("\t%s;\n", sym);
      }

  if (empty)
    {
      for (l in libs)
	if (inherit[l] == lib)
	  inherit[l] = inherit[lib];
    }
  else if (inherit[lib])
    printf("} %s;\n", inherit[lib]);
  else
    printf ("\n  local:\n\t*;\n};\n");
}
