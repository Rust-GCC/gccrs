enum Foo<'a> {
    Bar { field: &'a u32 }
}

fn in_let() {
    let y = 22;
    let foo = Foo::Bar { field: &y };
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    let Foo::Bar::<'static> { field: _z } = foo;
}

fn in_match() {
    let y = 22;
    let foo = Foo::Bar { field: &y };
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    match foo {
        Foo::Bar::<'static> { field: _z } => {
        }
    }
}

fn main() { }

