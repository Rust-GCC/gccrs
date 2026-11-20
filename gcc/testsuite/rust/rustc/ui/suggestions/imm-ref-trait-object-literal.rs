trait Trait {}

struct S;

impl<'a> Trait for &'a mut S {}

fn foo<X: Trait>(_: X) {}


fn main() {
  let s = S;
  foo(&s); // { dg-error ".E0277." "" { target *-*-* } }
  foo(s); // { dg-error ".E0277." "" { target *-*-* } }
}

