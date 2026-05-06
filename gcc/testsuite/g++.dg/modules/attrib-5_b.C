// PR c++/122786
// { dg-additional-options "-fmodules" }
// { dg-require-effective-target named_sections }

import M;

int main() {
  a();
  b();
}

// { dg-final { scan-assembler {barbar} } }
// { dg-final { scan-assembler {quxqux} } }
