// Regression test for #70934

struct S;

fn foo() {
    &([S][0],);
// { dg-error ".E0508." "" { target *-*-* } .-1 }
}

fn main() {}

