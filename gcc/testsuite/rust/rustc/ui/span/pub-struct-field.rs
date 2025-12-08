// Regression test for issue #26083 and #35435
// Test that span for public struct fields start at `pub`

struct Foo {
    bar: u8,
    pub bar: u8, // { dg-error ".E0124." "" { target *-*-* } }
    pub(crate) bar: u8, // { dg-error ".E0124." "" { target *-*-* } }
}

fn main() {}

