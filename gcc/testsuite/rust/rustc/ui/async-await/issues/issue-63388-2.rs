// edition:2018

struct Xyz {
    a: u64,
}

trait Foo {}

impl Xyz {
    async fn do_sth<'a>(
        foo: &dyn Foo, bar: &'a dyn Foo
    ) -> &dyn Foo // { dg-error ".E0106." "" { target *-*-* } }
    {
        foo
    }
}

fn main() {}

