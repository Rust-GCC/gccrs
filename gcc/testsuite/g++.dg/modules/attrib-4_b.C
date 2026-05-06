// PR c++/124483
// { dg-additional-options "-fmodules -Wunused-parameter" }

import B;

int main() {
  foo(123);
}
