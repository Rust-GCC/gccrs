trait Foo {
  fn answer(self);
}

struct NoData<T>;
// { dg-error ".E0392." "" { target *-*-* } .-1 }

impl<T> Foo for T where NoData<T>: Foo {
// { dg-error ".E0275." "" { target *-*-* } .-1 }
// { dg-error ".E0275." "" { target *-*-* } .-2 }
  fn answer(self) {
    let val: NoData<T> = NoData;
  }
}

fn main() {}

