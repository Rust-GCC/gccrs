// PR c++/100903
// { dg-additional-options -Wzero-as-null-pointer-constant }
// { dg-do compile { target c++20 } }

#include <compare>

int main()
{
  return (1.0 <=> 2.0) < 0;
}
