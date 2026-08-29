trait Trait {
    type Bar;
}

type Foo = dyn Trait; // { dg-error ".E0191." "" { target *-*-* } }

fn main() {}

