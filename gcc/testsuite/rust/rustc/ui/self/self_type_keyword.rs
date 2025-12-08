mod foo {
  struct Self;
// { dg-error "" "" { target *-*-* } .-1 }
}

struct Bar<'Self>;
// { dg-error ".E0392." "" { target *-*-* } .-1 }
// { dg-error ".E0392." "" { target *-*-* } .-2 }

struct Foo;

pub fn main() {
    match 15 {
        ref Self => (),
// { dg-error "" "" { target *-*-* } .-1 }
        mut Self => (),
// { dg-error ".E0531." "" { target *-*-* } .-1 }
// { dg-error ".E0531." "" { target *-*-* } .-2 }
        ref mut Self => (),
// { dg-error "" "" { target *-*-* } .-1 }
        Self!() => (),
// { dg-error "" "" { target *-*-* } .-1 }
        Foo { Self } => (),
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

mod m1 {
    extern crate core as Self;
// { dg-error "" "" { target *-*-* } .-1 }
}

mod m2 {
    use std::option::Option as Self;
// { dg-error "" "" { target *-*-* } .-1 }
}

mod m3 {
    trait Self {}
// { dg-error "" "" { target *-*-* } .-1 }
}

