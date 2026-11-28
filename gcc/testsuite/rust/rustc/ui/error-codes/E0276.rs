trait Foo {
    fn foo<T>(x: T);
}

impl Foo for bool {
    fn foo<T>(x: T) where T: Copy {} // { dg-error ".E0276." "" { target *-*-* } }
}

fn main() {
}

