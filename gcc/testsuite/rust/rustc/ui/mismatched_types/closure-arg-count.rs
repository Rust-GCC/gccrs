#![feature(unboxed_closures)]

fn f<F: Fn<usize>>(_: F) {}
fn main() {
    [1, 2, 3].sort_by(|| panic!());
// { dg-error ".E0593." "" { target *-*-* } .-1 }
    [1, 2, 3].sort_by(|tuple| panic!());
// { dg-error ".E0593." "" { target *-*-* } .-1 }
    [1, 2, 3].sort_by(|(tuple, tuple2)| panic!());
// { dg-error ".E0593." "" { target *-*-* } .-1 }
    [1, 2, 3].sort_by(|(tuple, tuple2): (usize, _)| panic!());
// { dg-error ".E0593." "" { target *-*-* } .-1 }
    f(|| panic!());
// { dg-error ".E0593." "" { target *-*-* } .-1 }
    f(  move    || panic!());
// { dg-error ".E0593." "" { target *-*-* } .-1 }

    let _it = vec![1, 2, 3].into_iter().enumerate().map(|i, x| i);
// { dg-error ".E0593." "" { target *-*-* } .-1 }
    let _it = vec![1, 2, 3].into_iter().enumerate().map(|i: usize, x| i);
// { dg-error ".E0593." "" { target *-*-* } .-1 }
    let _it = vec![1, 2, 3].into_iter().enumerate().map(|i, x, y| i);
// { dg-error ".E0593." "" { target *-*-* } .-1 }
    let _it = vec![1, 2, 3].into_iter().enumerate().map(foo);
// { dg-error ".E0593." "" { target *-*-* } .-1 }
    let bar = |i, x, y| i;
    let _it = vec![1, 2, 3].into_iter().enumerate().map(bar);
// { dg-error ".E0593." "" { target *-*-* } .-1 }
    let _it = vec![1, 2, 3].into_iter().enumerate().map(qux);
// { dg-error ".E0593." "" { target *-*-* } .-1 }

    let _it = vec![1, 2, 3].into_iter().map(usize::checked_add);
// { dg-error ".E0593." "" { target *-*-* } .-1 }

    call(Foo);
// { dg-error ".E0593." "" { target *-*-* } .-1 }
}

fn foo() {}
fn qux(x: usize, y: usize) {}

fn call<F, R>(_: F) where F: FnOnce() -> R {}
struct Foo(u8);

