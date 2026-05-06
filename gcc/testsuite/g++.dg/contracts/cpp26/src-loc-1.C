// { dg-do run { target c++23 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe " }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
// Test source location with #include <source_location>
#include <source_location>

int
foo (int x)
  pre ( x > 10 )
{
  return x - 9;
}

int main ()
{
  foo (9);
}

// { dg-output "contract violation in function int foo.int. at .*:9: x > 10.*(\n|\r\n|\r)" }
