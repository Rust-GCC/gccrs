// Test that non-inline modules are not allowed inside blocks.

fn main() {
    mod foo; // { dg-error "" "" { target *-*-* } }
}

