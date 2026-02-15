// Test that the 'static bound from the Copy impl is respected. Regression test for #29149.

#[derive(Clone)]
struct Foo<'a>(&'a u32);
impl Copy for Foo<'static> {}

fn main() {
    let s = 2;
    let a = (Foo(&s),); // { dg-error ".E0597." "" { target *-*-* } }
    drop(a.0);
    drop(a.0);
}

