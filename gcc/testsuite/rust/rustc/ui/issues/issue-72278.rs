// run-pass

#![allow(unused)]

struct S;

impl S {
    fn func<'a, U>(&'a self) -> U {
        todo!()
    }
}

fn dont_crash<'a, U>() -> U {
    S.func::<'a, U>()
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

fn main() {}

