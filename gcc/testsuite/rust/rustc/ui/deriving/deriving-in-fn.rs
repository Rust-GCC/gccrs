// run-pass
pub fn main() {
    #[derive(Debug)]
    struct Foo {
        foo: isize,
    }

    let f = Foo { foo: 10 };
    format!("{:?}", f);
}

