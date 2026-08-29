trait Qiz {
  fn qiz();
}

struct Foo;
impl Qiz for Foo {
  fn qiz() {}
}

struct Bar {
  foos: &'static [&'static (dyn Qiz + 'static)]
// { dg-error ".E0038." "" { target *-*-* } .-1 }
}

const FOO : Foo = Foo;
const BAR : Bar = Bar { foos: &[&FOO]};

fn main() { }

