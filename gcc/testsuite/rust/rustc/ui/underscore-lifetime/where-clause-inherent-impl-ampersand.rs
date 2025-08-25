// revisions: rust2015 rust2018
//[rust2018] edition:2018

trait WithType<T> {}
trait WithRegion<'a> { }

struct Foo<T> {
    t: T
}

impl<T> Foo<T>
where
    T: WithType<&u32>
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
{ }

fn main() {}

