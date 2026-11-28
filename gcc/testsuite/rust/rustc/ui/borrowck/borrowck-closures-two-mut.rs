// Tests that two closures cannot simultaneously have mutable
// access to the variable, whether that mutable access be used
// for direct assignment or for taking mutable ref. Issue #6801.

#![feature(box_syntax)]

fn to_fn_mut<F: FnMut()>(f: F) -> F { f }

fn a() {
    let mut x = 3;
    let c1 = to_fn_mut(|| x = 4);
    let c2 = to_fn_mut(|| x = 5); // { dg-error ".E0499." "" { target *-*-* } }
    drop((c1, c2));
}

fn set(x: &mut isize) {
    *x = 4;
}

fn b() {
    let mut x = 3;
    let c1 = to_fn_mut(|| set(&mut x));
    let c2 = to_fn_mut(|| set(&mut x)); // { dg-error ".E0499." "" { target *-*-* } }
    drop((c1, c2));
}

fn c() {
    let mut x = 3;
    let c1 = to_fn_mut(|| x = 5);
    let c2 = to_fn_mut(|| set(&mut x)); // { dg-error ".E0499." "" { target *-*-* } }
    drop((c1, c2));
}

fn d() {
    let mut x = 3;
    let c1 = to_fn_mut(|| x = 5);
    let c2 = to_fn_mut(|| { let _y = to_fn_mut(|| set(&mut x)); }); // (nested closure)
// { dg-error ".E0499." "" { target *-*-* } .-1 }
    drop((c1, c2));
}

fn g() {
    struct Foo {
        f: Box<isize>
    }

    let mut x: Box<_> = box Foo { f: box 3 };
    let c1 = to_fn_mut(|| set(&mut *x.f));
    let c2 = to_fn_mut(|| set(&mut *x.f));
// { dg-error ".E0499." "" { target *-*-* } .-1 }
    drop((c1, c2));
}

fn main() {
}

