// Forbidding a group (here, `unused`) overrules subsequent allowance of both
// the group, and an individual lint in the group (here, `unused_variables`);
// and, forbidding an individual lint (here, `non_snake_case`) overrules
// subsequent allowance of a lint group containing it (here, `nonstandard_style`). See
// Issue #42873.

#![forbid(unused, non_snake_case)]

#[allow(unused_variables)] // { dg-error ".E0453." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
fn foo() {}

#[allow(unused)] // { dg-error ".E0453." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
fn bar() {}

#[allow(nonstandard_style)] // { dg-error ".E0453." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
fn main() {
    println!("hello forbidden world")
}

