// Test `?Sized` local variables.

trait T {}

fn f1<W: ?Sized, X: ?Sized, Y: ?Sized, Z: ?Sized>(x: &X) {
    let _: W; // <-- this is OK, no bindings created, no initializer.
    let _: (isize, (X, isize));
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let y: Y;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let y: (isize, (Z, usize));
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}
fn f2<X: ?Sized, Y: ?Sized>(x: &X) {
    let y: X;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let y: (isize, (Y, isize));
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn f3<X: ?Sized>(x1: Box<X>, x2: Box<X>, x3: Box<X>) {
    let y: X = *x1;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let y = *x2;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let (y, z) = (*x3, 4);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}
fn f4<X: ?Sized + T>(x1: Box<X>, x2: Box<X>, x3: Box<X>) {
    let y: X = *x1;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let y = *x2;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let (y, z) = (*x3, 4);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn g1<X: ?Sized>(x: X) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
fn g2<X: ?Sized + T>(x: X) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

pub fn main() {
}

