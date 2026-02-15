struct Foo;

fn main() {
  let mut a = Foo;
  let ref b = Foo;
  a += *b; // { dg-error ".E0368." "" { target *-*-* } }
}

