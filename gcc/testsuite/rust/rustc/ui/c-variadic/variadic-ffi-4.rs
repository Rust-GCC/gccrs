#![crate_type = "lib"]
#![no_std]
#![feature(c_variadic)]

use core::ffi::{VaList, VaListImpl};

pub unsafe extern "C" fn no_escape0<'f>(_: usize, ap: ...) -> VaListImpl<'f> {
    ap
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

pub unsafe extern "C" fn no_escape1(_: usize, ap: ...) -> VaListImpl<'static> {
    ap // { dg-error "" "" { target *-*-* } }
}

pub unsafe extern "C" fn no_escape2(_: usize, ap: ...) {
    let _ = ap.with_copy(|ap| ap); // { dg-error "" "" { target *-*-* } }
}

pub unsafe extern "C" fn no_escape3(_: usize, mut ap0: &mut VaListImpl, mut ap1: ...) {
    *ap0 = ap1;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

pub unsafe extern "C" fn no_escape4(_: usize, mut ap0: &mut VaListImpl, mut ap1: ...) {
    ap0 = &mut ap1;
// { dg-error ".E0597." "" { target *-*-* } .-1 }
// { dg-error ".E0597." "" { target *-*-* } .-2 }
// { dg-error ".E0597." "" { target *-*-* } .-3 }
}

pub unsafe extern "C" fn no_escape5(_: usize, mut ap0: &mut VaListImpl, mut ap1: ...) {
    *ap0 = ap1.clone();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

