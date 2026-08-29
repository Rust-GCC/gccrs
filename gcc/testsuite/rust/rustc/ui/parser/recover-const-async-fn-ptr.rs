// edition:2018

type T0 = const fn(); // { dg-error "" "" { target *-*-* } }
type T1 = const extern "C" fn(); // { dg-error "" "" { target *-*-* } }
type T2 = const unsafe extern fn(); // { dg-error "" "" { target *-*-* } }
type T3 = async fn(); // { dg-error "" "" { target *-*-* } }
type T4 = async extern fn(); // { dg-error "" "" { target *-*-* } }
type T5 = async unsafe extern "C" fn(); // { dg-error "" "" { target *-*-* } }
type T6 = const async unsafe extern "C" fn();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

type FT0 = for<'a> const fn(); // { dg-error "" "" { target *-*-* } }
type FT1 = for<'a> const extern "C" fn(); // { dg-error "" "" { target *-*-* } }
type FT2 = for<'a> const unsafe extern fn(); // { dg-error "" "" { target *-*-* } }
type FT3 = for<'a> async fn(); // { dg-error "" "" { target *-*-* } }
type FT4 = for<'a> async extern fn(); // { dg-error "" "" { target *-*-* } }
type FT5 = for<'a> async unsafe extern "C" fn(); // { dg-error "" "" { target *-*-* } }
type FT6 = for<'a> const async unsafe extern "C" fn();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {
    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

