fn main() {
    test::<FooS>(&mut 42); // { dg-error "" "" { target *-*-* } }
}

trait Foo<'a> {}

struct FooS<'a> {
    data: &'a mut u32,
}

impl<'a, 'b: 'a> Foo<'b> for FooS<'a> {}

fn test<'a, F>(data: &'a mut u32) where F: for<'b> Foo<'b> {}

