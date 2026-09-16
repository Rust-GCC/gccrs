// The non-crate level cases are in issue-43106-gating-of-builtin-attrs.rs.

#![test                    = "4200"]
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

