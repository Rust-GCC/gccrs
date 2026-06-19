trait Foo {}
impl<'a, T> Foo for &'a T {}

struct Ctx<'a>(&'a ())
where
    &'a (): Foo, // { dg-error ".E0283." "" { target *-*-* } }
    &'static (): Foo;

fn main() {}

