#![feature(impl_trait_in_bindings)]
// { dg-warning "" "" { target *-*-* } .-1 }

const FOO: impl Copy = 42;

static BAR: impl Copy = 42;

fn main() {
    let foo: impl Copy = 42;

    let _ = FOO.count_ones();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    let _ = BAR.count_ones();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    let _ = foo.count_ones();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

