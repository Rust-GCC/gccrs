mod a {
    fn f() {}
}

fn main() {
    a::f(); // { dg-error ".E0603." "" { target *-*-* } }
}

