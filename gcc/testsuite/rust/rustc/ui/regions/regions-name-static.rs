struct Foo<'static> { // { dg-error ".E0262." "" { target *-*-* } }
    x: &'static isize
}

fn main() {}

