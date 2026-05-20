Various tests in this directory directly use the libstdc++ headers,
providing an end-to-end test of the interaction of -fanalyzer and
libstdc++ at various optimization levels.

libstdc++-v3/doc/xml/manual/using.xml
(https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_macros.html)
notes that by default, glibcxx assertions are enabled at -O0, and
disabled at -O1 and above.

We will explicitly disable them in some tests via:
  // { dg-additional-options "-D_GLIBCXX_NO_ASSERTIONS" }

With optimizations, many tests simplify to be within the analyzer's
normal limits, however at -O0 we may need to turn up
  --param=analyzer-supernode-explosion-factor=
for the analyzer to fully explore the code.
