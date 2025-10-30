// Don't allow tool_lints, which aren't scoped


#![deny(unknown_lints)]

#![deny(clippy)] // { dg-error "" "" { target *-*-* } }

fn main() {}

