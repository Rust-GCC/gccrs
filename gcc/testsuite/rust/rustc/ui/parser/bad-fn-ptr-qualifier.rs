// run-rustfix
// edition:2018
// Most of items are taken from ./recover-const-async-fn-ptr.rs but this is able to apply rustfix.

pub type T0 = const fn(); // { dg-error "" "" { target *-*-* } }
pub type T1 = const extern "C" fn(); // { dg-error "" "" { target *-*-* } }
pub type T2 = const unsafe extern fn(); // { dg-error "" "" { target *-*-* } }
pub type T3 = async fn(); // { dg-error "" "" { target *-*-* } }
pub type T4 = async extern fn(); // { dg-error "" "" { target *-*-* } }
pub type T5 = async unsafe extern "C" fn(); // { dg-error "" "" { target *-*-* } }
pub type T6 = const async unsafe extern "C" fn();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

pub type FTT0 = for<'a> const fn(); // { dg-error "" "" { target *-*-* } }
pub type FTT1 = for<'a> const extern "C" fn(); // { dg-error "" "" { target *-*-* } }
pub type FTT2 = for<'a> const unsafe extern fn(); // { dg-error "" "" { target *-*-* } }
pub type FTT3 = for<'a> async fn(); // { dg-error "" "" { target *-*-* } }
pub type FTT4 = for<'a> async extern fn(); // { dg-error "" "" { target *-*-* } }
pub type FTT5 = for<'a> async unsafe extern "C" fn();
// { dg-error "" "" { target *-*-* } .-1 }
pub type FTT6 = for<'a> const async unsafe extern "C" fn();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

