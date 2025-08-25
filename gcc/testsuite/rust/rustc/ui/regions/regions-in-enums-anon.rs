// Test that anonymous lifetimes are not permitted in enum declarations

enum Foo {
    Bar(&isize) // { dg-error ".E0106." "" { target *-*-* } }
}

fn main() {}

