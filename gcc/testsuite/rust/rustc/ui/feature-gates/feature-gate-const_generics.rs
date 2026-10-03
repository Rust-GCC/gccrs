fn foo<const X: ()>() {} // { dg-error ".E0658." "" { target *-*-* } }

struct Foo<const X: usize>([(); X]); // { dg-error ".E0658." "" { target *-*-* } }

fn main() {}

