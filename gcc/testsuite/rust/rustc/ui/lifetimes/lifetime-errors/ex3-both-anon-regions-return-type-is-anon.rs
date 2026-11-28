struct Foo {
  field: i32
}

impl Foo {
  fn foo<'a>(&self, x: &i32) -> &i32 {
    x // { dg-error ".E0623." "" { target *-*-* } }
  }
}

fn main() { }

