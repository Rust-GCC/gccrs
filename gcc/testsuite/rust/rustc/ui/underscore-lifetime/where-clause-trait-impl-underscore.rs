// revisions: rust2015 rust2018
//[rust2018] edition:2018

trait WithType<T> {}
trait WithRegion<'a> { }

trait Foo { }

impl<T> Foo for Vec<T>
where
    T: WithRegion<'_>
// { dg-error "" "" { target *-*-* } .-1 }
{ }

fn main() {}

