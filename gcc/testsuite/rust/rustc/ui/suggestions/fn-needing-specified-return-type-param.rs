fn f<A>() -> A { unimplemented!() }
fn foo() {
    let _ = f; // { dg-error ".E0282." "" { target *-*-* } }
}
fn main() {}

