#![feature(nll)]

struct Foo<'a, 'b: 'a>(&'a &'b ());

impl<'a, 'b> Foo<'a, 'b> {
    fn xmute(a: &'b ()) -> &'a () {
        unreachable!()
    }
}

pub fn foo<'a, 'b>(u: &'b ()) -> &'a () {
    Foo::xmute(u) // { dg-error "" "" { target *-*-* } }
}

fn main() {}

