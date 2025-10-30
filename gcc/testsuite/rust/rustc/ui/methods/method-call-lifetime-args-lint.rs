#![deny(late_bound_lifetime_arguments)]
#![allow(unused)]

struct S;

impl S {
    fn late<'a, 'b>(self, _: &'a u8, _: &'b u8) {}
    fn late_implicit(self, _: &u8, _: &u8) {}
}

fn method_call() {
    S.late::<'static>(&0, &0);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    S.late_implicit::<'static>(&0, &0);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

fn main() {}

