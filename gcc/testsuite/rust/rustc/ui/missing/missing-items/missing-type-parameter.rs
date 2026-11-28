fn foo<X>() { }

fn main() {
    foo(); // { dg-error ".E0282." "" { target *-*-* } }
}

