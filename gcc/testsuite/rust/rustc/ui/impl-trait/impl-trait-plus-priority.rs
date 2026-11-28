// compile-flags: -Z parse-only

fn f() -> impl A + {} // OK
fn f() -> impl A + B {} // OK
fn f() -> dyn A + B {} // OK
fn f() -> A + B {} // OK

impl S {
    fn f(self) -> impl A + { // OK
        let _ = |a, b| -> impl A + {}; // OK
    }
    fn f(self) -> impl A + B { // OK
        let _ = |a, b| -> impl A + B {}; // OK
    }
    fn f(self) -> dyn A + B { // OK
        let _ = |a, b| -> dyn A + B {}; // OK
    }
    fn f(self) -> A + B { // OK
        let _ = |a, b| -> A + B {}; // OK
    }
}

type A = fn() -> impl A +;
// { dg-error "" "" { target *-*-* } .-1 }
type A = fn() -> impl A + B;
// { dg-error "" "" { target *-*-* } .-1 }
type A = fn() -> dyn A + B;
// { dg-error "" "" { target *-*-* } .-1 }
type A = fn() -> A + B;
// { dg-error ".E0178." "" { target *-*-* } .-1 }

type A = Fn() -> impl A +;
// { dg-error "" "" { target *-*-* } .-1 }
type A = Fn() -> impl A + B;
// { dg-error "" "" { target *-*-* } .-1 }
type A = Fn() -> dyn A + B;
// { dg-error "" "" { target *-*-* } .-1 }
type A = Fn() -> A + B; // OK, interpreted as `(Fn() -> A) + B` for compatibility

type A = &impl A +;
// { dg-error "" "" { target *-*-* } .-1 }
type A = &impl A + B;
// { dg-error "" "" { target *-*-* } .-1 }
type A = &dyn A + B;
// { dg-error "" "" { target *-*-* } .-1 }
type A = &A + B;
// { dg-error ".E0178." "" { target *-*-* } .-1 }

fn main() {}

