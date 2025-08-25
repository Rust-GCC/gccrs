#![feature(extern_types)]

fn main() {}

extern "C" {
    type A: Ord;
// { dg-error "" "" { target *-*-* } .-1 }
    type B<'a> where 'a: 'static;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    type C<T: Ord> where T: 'static;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    type D = u8;
// { dg-error "" "" { target *-*-* } .-1 }

    type E: where;
}

