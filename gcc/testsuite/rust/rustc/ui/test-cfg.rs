// compile-flags: --cfg foo

#[cfg(all(foo, bar))] // foo AND bar
fn foo() {}

fn main() {
    foo(); // { dg-error ".E0425." "" { target *-*-* } }
}

