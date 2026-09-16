trait Foo<'a> {
    const C: &'a u32;
}

impl<'a, T> Foo<'a> for T {
    const C: &'a u32 = &22;
}

fn foo<'a, T: Foo<'a>>() -> &'static u32 {
    T::C // { dg-error ".E0495." "" { target *-*-* } }
}

fn main() {
}

