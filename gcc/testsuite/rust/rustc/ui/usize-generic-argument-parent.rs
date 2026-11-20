fn foo() {
    let x: usize<foo>; // { dg-error ".E0109." "" { target *-*-* } }
}

fn main() {}

