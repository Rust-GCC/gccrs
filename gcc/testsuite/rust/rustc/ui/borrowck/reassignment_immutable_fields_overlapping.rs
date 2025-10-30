// This should never be allowed -- `foo.a` and `foo.b` are
// overlapping, so since `x` is not `mut` we should not permit
// reassignment.

union Foo {
    a: u32,
    b: u32,
}

unsafe fn overlapping_fields() {
    let x: Foo;
    x.a = 1;  // { dg-error ".E0381." "" { target *-*-* } }
    x.b = 22; // { dg-error ".E0594." "" { target *-*-* } }
}

fn main() { }

