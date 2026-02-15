// Testing that crate-level `#![macro_escape]` is not gated beyond a
// depecation warning.  This file sits on its own, because crate-level
// `#![macro_escape]` is incompatible with crate-level `#![macro_use]`
// already present in issue-43106-gating-of-builtin-attrs.

// check-pass

#![macro_escape]
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {}

