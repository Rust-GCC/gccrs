struct Foo;

fn main() {
    || if let Foo::NotEvenReal() = Foo {}; // { dg-error ".E0599." "" { target *-*-* } }
}

