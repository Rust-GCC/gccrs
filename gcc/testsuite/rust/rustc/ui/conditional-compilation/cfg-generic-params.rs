// compile-flags:--cfg yes

fn f_lt<#[cfg(yes)] 'a: 'a, #[cfg(no)] T>() {}
fn f_ty<#[cfg(no)] 'a: 'a, #[cfg(yes)] T>() {}

type FnGood = for<#[cfg(yes)] 'a, #[cfg(no)] T> fn(); // OK
type FnBad = for<#[cfg(no)] 'a, #[cfg(yes)] T> fn();
// { dg-error "" "" { target *-*-* } .-1 }

type PolyGood = dyn for<#[cfg(yes)] 'a, #[cfg(no)] T> Copy; // OK
type PolyBad = dyn for<#[cfg(no)] 'a, #[cfg(yes)] T> Copy;
// { dg-error "" "" { target *-*-* } .-1 }

struct WhereGood where for<#[cfg(yes)] 'a, #[cfg(no)] T> u8: Copy; // OK
struct WhereBad where for<#[cfg(no)] 'a, #[cfg(yes)] T> u8: Copy;
// { dg-error "" "" { target *-*-* } .-1 }

fn f_lt_no<#[cfg_attr(no, unknown)] 'a>() {} // OK
fn f_lt_yes<#[cfg_attr(yes, unknown)] 'a>() {}
// { dg-error "" "" { target *-*-* } .-1 }
fn f_ty_no<#[cfg_attr(no, unknown)] T>() {} // OK
fn f_ty_yes<#[cfg_attr(yes, unknown)] T>() {}
// { dg-error "" "" { target *-*-* } .-1 }

type FnNo = for<#[cfg_attr(no, unknown)] 'a> fn(); // OK
type FnYes = for<#[cfg_attr(yes, unknown)] 'a> fn();
// { dg-error "" "" { target *-*-* } .-1 }

type PolyNo = dyn for<#[cfg_attr(no, unknown)] 'a> Copy; // OK
type PolyYes = dyn for<#[cfg_attr(yes, unknown)] 'a> Copy;
// { dg-error "" "" { target *-*-* } .-1 }

struct WhereNo where for<#[cfg_attr(no, unknown)] 'a> u8: Copy; // OK
struct WhereYes where for<#[cfg_attr(yes, unknown)] 'a> u8: Copy;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    f_lt::<'static>();
    f_ty::<u8>();
}

