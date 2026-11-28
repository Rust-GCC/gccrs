trait Trait {
    fn dummy(&self) { }
}

struct Foo<T:Trait> {
    x: T,
}

fn main() {
    let foo = Foo {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        x: 3
    };

    let baz: Foo<usize> = loop { };
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

