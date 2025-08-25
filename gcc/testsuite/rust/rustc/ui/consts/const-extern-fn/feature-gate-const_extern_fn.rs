// Check that `const extern fn` and `const unsafe extern fn` are feature-gated.

const extern fn foo1() {} // { dg-error ".E0658." "" { target *-*-* } }
const extern "C" fn foo2() {} // { dg-error ".E0658." "" { target *-*-* } }
const extern "Rust" fn foo3() {} // { dg-error ".E0658." "" { target *-*-* } }
const unsafe extern fn bar1() {} // { dg-error ".E0658." "" { target *-*-* } }
const unsafe extern "C" fn bar2() {} // { dg-error ".E0658." "" { target *-*-* } }
const unsafe extern "Rust" fn bar3() {} // { dg-error ".E0658." "" { target *-*-* } }

fn main() {}

