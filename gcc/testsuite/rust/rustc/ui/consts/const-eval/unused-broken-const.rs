// make sure that an *unused* broken const triggers an error even in a check build

// compile-flags: --emit=dep-info,metadata

const FOO: i32 = [][0];
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

