#![allow(non_camel_case_types)]

mod foo { pub struct bar; }

fn main() {
    let bar = 5;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    use foo::bar;
}

