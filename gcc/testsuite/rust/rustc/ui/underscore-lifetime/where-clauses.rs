trait Foo<'a> {}

impl<'b: '_> Foo<'b> for i32 {} // { dg-error ".E0637." "" { target *-*-* } }

impl<T: '_> Foo<'static> for Vec<T> {} // { dg-error ".E0637." "" { target *-*-* } }

fn main() { }

