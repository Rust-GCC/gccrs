mod bar {
    pub struct Foo {
        pub a: isize,
        b: isize,
    }

    pub struct FooTuple (
        pub isize,
        isize,
    );
}

fn pat_match(foo: bar::Foo) {
    let bar::Foo{a, b} = foo; // { dg-error ".E0451." "" { target *-*-* } }
}

fn main() {
    let f = bar::Foo{ a: 0, b: 0 }; // { dg-error ".E0451." "" { target *-*-* } }
}

