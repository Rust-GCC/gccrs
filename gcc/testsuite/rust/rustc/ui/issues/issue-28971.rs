enum Foo {
    Bar(u8)
}
fn main(){
    foo(|| {
        match Foo::Bar(1) {
            Foo::Baz(..) => (),
// { dg-error ".E0599." "" { target *-*-* } .-1 }
            _ => (),
        }
    });
}

fn foo<F>(f: F) where F: FnMut() {
    f();
}

