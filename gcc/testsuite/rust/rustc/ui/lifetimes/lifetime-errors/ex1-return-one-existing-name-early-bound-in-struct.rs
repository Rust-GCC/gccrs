#[derive(Clone)]
enum Foo<'a> {
    Bar(&'a str),
}

impl<'a> Foo<'a> {
    fn bar(&self, other: Foo) -> Foo<'a> {
        match *self {
            Foo::Bar(s) => {
                if s == "test" {
                    other // { dg-error ".E0621." "" { target *-*-* } }
                } else {
                    self.clone()
                }
            }
        }
    }
}

fn main() { }

