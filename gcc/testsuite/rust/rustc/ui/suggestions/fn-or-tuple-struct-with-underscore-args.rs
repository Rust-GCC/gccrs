fn foo(a: usize, b: usize) -> usize { a }

struct S(usize, usize);

trait T {
    fn baz(x: usize, y: usize) -> usize { x }
}

fn main() {
    let _: usize = foo(_, _);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    let _: S = S(_, _);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    let _: usize = T::baz(_, _);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

