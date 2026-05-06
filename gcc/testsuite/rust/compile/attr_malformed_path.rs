#![feature(no_core)]
#![no_core]

#[cfg_attr(target_arch = "x86_64", path = (target_arch = "x86",    path = "x86.rs"))]
mod imp {}
// { dg-error "malformed .path. attribute input" "" { target { x86_64-*-* && { lp64 } } } .-2 }
// { dg-error "cannot find value .target_arch. in this scope" "" { target { x86_64-*-* && { lp64 } } } .-3 }
// { dg-error "cannot find value .path. in this scope" "" { target { x86_64-*-* && { lp64 } } } .-4 }
