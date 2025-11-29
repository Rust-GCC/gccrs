// Test that ! errors when used in illegal positions with feature(never_type) disabled

trait Foo {
    type Wub;
}

type Ma = (u32, !, i32); // { dg-error ".E0658." "" { target *-*-* } }
type Meeshka = Vec<!>; // { dg-error ".E0658." "" { target *-*-* } }
type Mow = &'static fn(!) -> !; // { dg-error ".E0658." "" { target *-*-* } }
type Skwoz = &'static mut !; // { dg-error ".E0658." "" { target *-*-* } }

impl Foo for Meeshka {
    type Wub = !; // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {
}

