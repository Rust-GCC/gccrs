struct Foo(bool);
fn foo(_: usize) -> Foo { Foo(false) }

fn main() {
    match Foo(true) {
        foo(x) // { dg-error ".E0532." "" { target *-*-* } }
        => ()
    }
}

