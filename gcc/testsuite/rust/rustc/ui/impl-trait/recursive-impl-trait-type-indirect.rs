// Test that impl trait does not allow creating recursive types that are
// otherwise forbidden.

#![feature(generators)]
#![allow(unconditional_recursion)]

fn option(i: i32) -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    if i < 0 { None } else { Some((option(i - 1), i)) }
}

fn tuple() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    (tuple(),)
}

fn array() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    [array()]
}

fn ptr() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    &ptr() as *const _
}

fn fn_ptr() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    fn_ptr as fn() -> _
}

fn closure_capture() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    let x = closure_capture();
    move || {
        x;
    }
}

fn closure_ref_capture() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    let x = closure_ref_capture();
    move || {
        &x;
    }
}

fn closure_sig() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    || closure_sig()
}

fn generator_sig() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    || generator_sig()
}

fn generator_capture() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    let x = generator_capture();
    move || {
        yield;
        x;
    }
}

fn substs_change<T: 'static>() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    (substs_change::<&T>(),)
}

fn generator_hold() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    move || {
        let x = generator_hold();
        yield;
        x;
    }
}

fn use_fn_ptr() -> impl Sized {
    // OK, error already reported
    fn_ptr()
}

fn mutual_recursion() -> impl Sync {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    mutual_recursion_b()
}

fn mutual_recursion_b() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    mutual_recursion()
}

fn main() {}

