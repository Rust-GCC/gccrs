fn foo<T>() -> T {
    panic!()
}

fn main() {
    foo(); // { dg-error ".E0282." "" { target *-*-* } }
}

