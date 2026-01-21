/* Test RISC-V Zicfilp C++ thunk LPAD.  */
/* { dg-do compile { target { riscv64*-*-* } } } */
/* { dg-options "-O2 -march=rv64gc_zicfilp -mabi=lp64d -fcf-protection=none -fcf-protection=branch" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-g" "-flto" } } */
/* { dg-final { check-function-bodies "**" "" {\.align|lpad} } } */

class Base1 {
public:
  virtual int method1 (int x) { return x + 1; }
  int data1;
};

class Base2 {
public:
  virtual int method2 (int x) { return x + 2; }
  int data2;
};

class Derived : public Base1, public Base2 {
public:
  virtual int method2 (int x) override { return x + 10; }
};

Derived global_derived;

int call_method2 (Base2 *p)
{
  return p->method2 (42);
}

/*
** _ZThn16_N7Derived7method2Ei:
**	\.align	2
**	lpad	\d+
**	...
*/
