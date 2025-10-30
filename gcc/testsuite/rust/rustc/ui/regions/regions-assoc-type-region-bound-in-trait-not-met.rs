// Test that the compiler checks that arbitrary region bounds declared
// in the trait must be satisfied on the impl. Issue #20890.

trait Foo<'a> {
    type Value: 'a;
    fn dummy(&'a self) {}
}

impl<'a> Foo<'a> for &'a i16 {
    // OK.
    type Value = &'a i32;
}

impl<'a> Foo<'static> for &'a i32 {
    type Value = &'a i32;
// { dg-error ".E0477." "" { target *-*-* } .-1 }
}

impl<'a, 'b> Foo<'b> for &'a i64 {
    type Value = &'a i32;
// { dg-error ".E0477." "" { target *-*-* } .-1 }
}

fn main() {}

