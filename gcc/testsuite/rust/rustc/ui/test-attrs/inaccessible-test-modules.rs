// compile-flags:--test

// the `--test` harness creates modules with these textual names, but
// they should be inaccessible from normal code.
use main as x; // { dg-error ".E0432." "" { target *-*-* } }
use test as y; // { dg-error ".E0432." "" { target *-*-* } }

#[test]
fn baz() {}

