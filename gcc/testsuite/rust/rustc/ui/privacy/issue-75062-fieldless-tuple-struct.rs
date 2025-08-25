// Regression test for issue #75062
// Tests that we don't ICE on a privacy error for a fieldless tuple struct.

mod foo {
    struct Bar();
}

fn main() {
    foo::Bar(); // { dg-error ".E0603." "" { target *-*-* } }
}

