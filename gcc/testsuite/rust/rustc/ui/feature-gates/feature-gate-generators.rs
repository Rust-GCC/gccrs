fn main() {
    yield true; // { dg-error ".E0627." "" { target *-*-* } }
// { dg-error ".E0627." "" { target *-*-* } .-1 }
}

#[cfg(FALSE)]
fn foo() {
    yield; // { dg-error ".E0658." "" { target *-*-* } }
    yield 0; // { dg-error ".E0658." "" { target *-*-* } }
}

