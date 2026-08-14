#[allow(unused_imports)]

mod foo {
    use baz::bar;
    mod bar {}
// { dg-error ".E0255." "" { target *-*-* } .-1 }
}
mod baz { pub mod bar {} }

fn main() {}

