use std::marker;

struct B<T>(marker::PhantomData<T>);

fn main() {
    let foo = B(marker::PhantomData); // { dg-error ".E0282." "" { target *-*-* } }
    let closure = || foo;
}

