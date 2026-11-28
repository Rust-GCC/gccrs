struct Foo<'a>(&'a u32);

fn in_let() {
    let y = 22;
    let foo = Foo(&y);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    let Foo::<'static>(_z) = foo;
}

fn in_match() {
    let y = 22;
    let foo = Foo(&y);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    match foo {
        Foo::<'static>(_z) => {
        }
    }
}

fn main() { }

