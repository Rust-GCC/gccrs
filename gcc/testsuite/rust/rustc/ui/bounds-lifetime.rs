type A = for<'b, 'a: 'b> fn(); // { dg-error "" "" { target *-*-* } }
type B = for<'b, 'a: 'b,> fn(); // { dg-error "" "" { target *-*-* } }
type C = for<'b, 'a: 'b +> fn(); // { dg-error "" "" { target *-*-* } }
type D = for<'a, T> fn(); // { dg-error "" "" { target *-*-* } }
type E = dyn for<T> Fn(); // { dg-error "" "" { target *-*-* } }

fn main() {}

