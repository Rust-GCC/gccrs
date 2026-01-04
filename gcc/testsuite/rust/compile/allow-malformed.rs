// { dg-options "-w" }
#[allow] // { dg-error "malformed .allow. attribute input" }
fn foo() {}

// { dg-note "must be of the form" "" { target *-*-* } .-3 }
