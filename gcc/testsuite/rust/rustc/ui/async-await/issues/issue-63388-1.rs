// edition:2018

struct Xyz {
    a: u64,
}

trait Foo {}

impl Xyz {
    async fn do_sth<'a>(
        &'a self, foo: &dyn Foo
    ) -> &dyn Foo
    {
        foo  // { dg-error ".E0623." "" { target *-*-* } }
    }
}

fn main() {}

