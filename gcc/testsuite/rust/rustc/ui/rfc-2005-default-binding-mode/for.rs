struct Foo {}

pub fn main() {
    let mut tups = vec![(Foo {}, Foo {})];
    // The below desugars to &(ref n, mut m).
    for (n, mut m) in &tups {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
    }
}

