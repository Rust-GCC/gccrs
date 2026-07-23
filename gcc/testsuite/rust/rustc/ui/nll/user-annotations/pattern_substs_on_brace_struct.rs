struct Foo<'a> { field: &'a u32 }

fn in_let() {
    let y = 22;
    let foo = Foo { field: &y };
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    let Foo::<'static> { field: _z } = foo;
}

fn in_main() {
    let y = 22;
    let foo = Foo { field: &y };
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    match foo {
        Foo::<'static> { field: _z } => {
        }
    }
}

fn main() { }

