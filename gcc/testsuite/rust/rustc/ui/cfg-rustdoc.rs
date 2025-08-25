#[cfg(doc)]
pub struct Foo;

fn main() {
    let f = Foo; // { dg-error ".E0425." "" { target *-*-* } }
}

