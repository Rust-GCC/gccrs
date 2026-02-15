// Test that anonymous lifetimes are not permitted in struct declarations

struct Foo {
    x: &isize // { dg-error ".E0106." "" { target *-*-* } }
}

fn main() {}

