// Test that (for now) we report an ambiguity error here, because
// specific trait relationships are ignored for the purposes of trait
// matching. This behavior should likely be improved such that this
// test passes. See #21974 for more details.

trait Foo {
    fn foo(self);
}

fn foo<'a,'b,T>(x: &'a T, y: &'b T)
    where &'a T : Foo, // { dg-error ".E0283." "" { target *-*-* } }
          &'b T : Foo
{
    x.foo();
    y.foo();
}

fn main() { }

