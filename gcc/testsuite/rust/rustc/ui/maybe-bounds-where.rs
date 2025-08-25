struct S1<T>(T) where (T): ?Sized;
// { dg-error "" "" { target *-*-* } .-1 }

struct S2<T>(T) where u8: ?Sized;
// { dg-error "" "" { target *-*-* } .-1 }

struct S3<T>(T) where &'static T: ?Sized;
// { dg-error "" "" { target *-*-* } .-1 }

trait Trait<'a> {}

struct S4<T>(T) where for<'a> T: ?Trait<'a>;
// { dg-error "" "" { target *-*-* } .-1 }

struct S5<T>(*const T) where T: ?Trait<'static> + ?Sized;
// { dg-error ".E0203." "" { target *-*-* } .-1 }
// { dg-warning ".E0203." "" { target *-*-* } .-2 }

impl<T> S1<T> {
    fn f() where T: ?Sized {}
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {
    let u = vec![1, 2, 3];
    let _s: S5<[u8]> = S5(&u[..]); // OK
}

