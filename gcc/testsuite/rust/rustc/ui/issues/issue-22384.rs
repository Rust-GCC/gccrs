trait Trait {
    fn foo();
}

fn main() {
    <<i32 as Copy>::foobar as Trait>::foo();
// { dg-error ".E0576." "" { target *-*-* } .-1 }
}

