trait Foo {}

impl<T: Fn(&())> Foo for T {}

fn baz<T: Foo>(_: T) {}

fn main() {
    baz(|_| ()); // { dg-error ".E0308." "" { target *-*-* } }
}

