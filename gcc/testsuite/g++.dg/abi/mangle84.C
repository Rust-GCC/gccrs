// PR c++/124768
// { dg-final { scan-assembler "two:" } }
// { dg-final { scan-assembler-not "_ZN12_GLOBAL__N_13twoE:" } }
namespace { extern "C" { int two = 2; } }
int main() { return two; }
