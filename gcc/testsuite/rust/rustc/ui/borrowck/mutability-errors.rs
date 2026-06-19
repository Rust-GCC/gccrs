// All the possible mutability error cases.

#![allow(unused)]

type MakeRef = fn() -> &'static (i32,);
type MakePtr = fn() -> *const (i32,);

fn named_ref(x: &(i32,)) {
    *x = (1,); // { dg-error ".E0594." "" { target *-*-* } }
    x.0 = 1; // { dg-error ".E0594." "" { target *-*-* } }
    &mut *x; // { dg-error ".E0596." "" { target *-*-* } }
    &mut x.0; // { dg-error ".E0596." "" { target *-*-* } }
}

fn unnamed_ref(f: MakeRef) {
    *f() = (1,); // { dg-error ".E0594." "" { target *-*-* } }
    f().0 = 1; // { dg-error ".E0594." "" { target *-*-* } }
    &mut *f(); // { dg-error ".E0596." "" { target *-*-* } }
    &mut f().0; // { dg-error ".E0596." "" { target *-*-* } }
}

unsafe fn named_ptr(x: *const (i32,)) {
    *x = (1,); // { dg-error ".E0594." "" { target *-*-* } }
    (*x).0 = 1; // { dg-error ".E0594." "" { target *-*-* } }
    &mut *x; // { dg-error ".E0596." "" { target *-*-* } }
    &mut (*x).0; // { dg-error ".E0596." "" { target *-*-* } }
}

unsafe fn unnamed_ptr(f: MakePtr) {
    *f() = (1,); // { dg-error ".E0594." "" { target *-*-* } }
    (*f()).0 = 1; // { dg-error ".E0594." "" { target *-*-* } }
    &mut *f(); // { dg-error ".E0596." "" { target *-*-* } }
    &mut (*f()).0; // { dg-error ".E0596." "" { target *-*-* } }
}

fn fn_ref<F: Fn()>(f: F) -> F { f }

fn ref_closure(mut x: (i32,)) {
    fn_ref(|| {
        x = (1,); // { dg-error ".E0594." "" { target *-*-* } }
        x.0 = 1; // { dg-error ".E0594." "" { target *-*-* } }
        &mut x; // { dg-error ".E0596." "" { target *-*-* } }
        &mut x.0; // { dg-error ".E0596." "" { target *-*-* } }
    });
    fn_ref(move || {
        x = (1,); // { dg-error ".E0594." "" { target *-*-* } }
        x.0 = 1; // { dg-error ".E0594." "" { target *-*-* } }
        &mut x; // { dg-error ".E0596." "" { target *-*-* } }
        &mut x.0; // { dg-error ".E0596." "" { target *-*-* } }
    });
}

fn imm_local(x: (i32,)) {
    &mut x; // { dg-error ".E0596." "" { target *-*-* } }
    &mut x.0; // { dg-error ".E0596." "" { target *-*-* } }
}

fn imm_capture(x: (i32,)) {
    || {
        x = (1,); // { dg-error ".E0594." "" { target *-*-* } }
        x.0 = 1; // { dg-error ".E0594." "" { target *-*-* } }
        &mut x; // { dg-error ".E0596." "" { target *-*-* } }
        &mut x.0; // { dg-error ".E0596." "" { target *-*-* } }
    };
    move || {
        x = (1,); // { dg-error ".E0594." "" { target *-*-* } }
        x.0 = 1; // { dg-error ".E0594." "" { target *-*-* } }
        &mut x; // { dg-error ".E0596." "" { target *-*-* } }
        &mut x.0; // { dg-error ".E0596." "" { target *-*-* } }
    };
}

static X: (i32,) = (0,);

fn imm_static() {
    X = (1,); // { dg-error ".E0594." "" { target *-*-* } }
    X.0 = 1; // { dg-error ".E0594." "" { target *-*-* } }
    &mut X; // { dg-error ".E0596." "" { target *-*-* } }
    &mut X.0; // { dg-error ".E0596." "" { target *-*-* } }
}

fn main() {}

