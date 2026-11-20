// Check that the visibility modifier is included in the span of foreign items.

extern {
  fn foo();

  pub // { dg-error ".E0428." "" { target *-*-* } }
  fn foo();

  pub // { dg-error ".E0428." "" { target *-*-* } }
  static mut foo: u32;
}

fn main() {
}

