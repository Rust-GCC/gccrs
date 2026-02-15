struct Foo<'a: '_>(&'a u8); // { dg-error ".E0637." "" { target *-*-* } }
fn foo<'a: '_>(_: &'a u8) {} // { dg-error ".E0637." "" { target *-*-* } }

struct Bar<'a>(&'a u8);
impl<'a: '_> Bar<'a> { // { dg-error ".E0637." "" { target *-*-* } }
  fn bar() {}
}

fn main() {}

