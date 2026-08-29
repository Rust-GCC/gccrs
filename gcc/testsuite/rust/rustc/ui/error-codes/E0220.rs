trait Trait {
    type Bar;
}

type Foo = dyn Trait<F=i32>; // { dg-error ".E0191." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
fn main() {
}

