trait Trait<'a, 'b> {}
impl<T> Trait<'_, '_> for T {}

fn foo<'a, 'b>(x: &'a u32, y: &'b u32) -> impl Trait<'a, 'b> {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    (x, y)
}

fn main() {}

