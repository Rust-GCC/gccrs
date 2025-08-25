struct Foo<'a> { x: &'a u32 }

impl<'a> Foo<'a> {
    const C: &'a u32 = &22;
}

fn foo<'a>(_: &'a u32) -> &'static u32 {
    <Foo<'a>>::C // { dg-error ".E0759." "" { target *-*-* } }
}

fn main() {
}

