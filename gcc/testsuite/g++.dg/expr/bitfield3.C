// PR c++/30274

struct S {
  bool x : 4;
};

S s;

void f() {
  s.x--; // { dg-error "5:use of an operand of type .bool." }
  --s.x; // { dg-error "7:use of an operand of type .bool." }
}
