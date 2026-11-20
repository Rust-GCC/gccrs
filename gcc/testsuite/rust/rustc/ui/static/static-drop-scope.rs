#![feature(const_fn)]

struct WithDtor;

impl Drop for WithDtor {
    fn drop(&mut self) {}
}

static PROMOTION_FAIL_S: Option<&'static WithDtor> = Some(&WithDtor);
// { dg-error ".E0493." "" { target *-*-* } .-1 }
// { dg-error ".E0716." "" { target *-*-* } .-2 }

const PROMOTION_FAIL_C: Option<&'static WithDtor> = Some(&WithDtor);
// { dg-error ".E0493." "" { target *-*-* } .-1 }
// { dg-error ".E0716." "" { target *-*-* } .-2 }

static EARLY_DROP_S: i32 = (WithDtor, 0).1;
// { dg-error ".E0493." "" { target *-*-* } .-1 }

const EARLY_DROP_C: i32 = (WithDtor, 0).1;
// { dg-error ".E0493." "" { target *-*-* } .-1 }

const fn const_drop<T>(_: T) {}
// { dg-error ".E0493." "" { target *-*-* } .-1 }

const fn const_drop2<T>(x: T) {
    (x, ()).1
// { dg-error ".E0493." "" { target *-*-* } .-1 }
}

const EARLY_DROP_C_OPTION: i32 = (Some(WithDtor), 0).1;
// { dg-error ".E0493." "" { target *-*-* } .-1 }

const HELPER: Option<WithDtor> = Some(WithDtor);

const EARLY_DROP_C_OPTION_CONSTANT: i32 = (HELPER, 0).1;
// { dg-error ".E0493." "" { target *-*-* } .-1 }

fn main () {}

