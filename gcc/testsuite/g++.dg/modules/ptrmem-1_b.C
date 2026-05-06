// PR c++/124981
// { dg-additional-options "-fmodules" }
// { dg-module-do link }

import M;

int main() {
  B<void> b;
  b.m.f(b);
}
