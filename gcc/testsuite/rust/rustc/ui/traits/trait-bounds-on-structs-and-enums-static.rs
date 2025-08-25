trait Trait {
    fn dummy(&self) { }
}

struct Foo<T:Trait> {
    x: T,
}

static X: Foo<usize> = Foo {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    x: 1,
};

fn main() {
}

