/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O1 -fPIC -fvisibility=internal -mtls-dialect=gnu" } */

__thread int err;
bool m;
struct Operand {
  int : 6;
  int kind_ : 10;
  int bit_ : 4;
  Operand(){};
};
struct Reg32e : Operand {};
Operand mm;
void f1();
void f(Reg32e B) {
  for (;;)
  {
    Operand &r = B;
    m = r.kind_ && r.bit_ & 1;
    if (err)
      err = 1;
    int t = r.bit_;
    if (t)
      mm = r;
    f1();
  }
}

/* { dg-final { scan-assembler-times "call\[ \t\]__tls_get_addr@PLT" 1 { target { ! ia32 } } } } */
