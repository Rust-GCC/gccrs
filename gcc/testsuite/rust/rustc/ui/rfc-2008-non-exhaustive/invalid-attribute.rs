#[non_exhaustive(anything)]
// { dg-error "" "" { target *-*-* } .-1 }
struct Foo;

#[non_exhaustive]
// { dg-error ".E0701." "" { target *-*-* } .-1 }
trait Bar { }

#[non_exhaustive]
// { dg-error ".E0701." "" { target *-*-* } .-1 }
union Baz {
    f1: u16,
    f2: u16
}

fn main() { }

