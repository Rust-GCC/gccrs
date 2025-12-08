// issue #41834

#[derive(Default)]
struct Foo {
    one: u8,
}

fn main() {
    let foo = Foo {
        one: 111,
        ..Foo::default(),
// { dg-error "" "" { target *-*-* } .-1 }
    };

    let foo = Foo {
        ..Foo::default(),
// { dg-error "" "" { target *-*-* } .-1 }
        one: 111,
    };
}

