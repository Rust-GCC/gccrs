struct Foo(u32);

fn test() -> Foo { Foo } // { dg-error ".E0308." "" { target *-*-* } }

fn main() {}

