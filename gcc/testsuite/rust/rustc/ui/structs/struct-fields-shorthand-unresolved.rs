struct Foo {
    x: i32,
    y: i32
}

fn main() {
    let x = 0;
    let foo = Foo {
        x,
        y // { dg-error ".E0425." "" { target *-*-* } }
    };
}

