// { dg-options "-w" }
#[link_section] // { dg-error "malformed .link_section. attribute input" }
pub static VAR1: u32 = 1;

// { dg-note "must be of the form" "" { target *-*-* } .-3 }
