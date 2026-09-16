mod a {
    struct S;
    impl S { }
}

fn foo(_: a::S) { // { dg-error ".E0603." "" { target *-*-* } }
}

fn main() {}

