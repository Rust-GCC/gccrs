mod a {
    struct A;

    impl Default for A {
        pub fn default() -> A { // { dg-error ".E0449." "" { target *-*-* } }
            A
        }
    }
}


fn main() {
    a::A::default();
// { dg-error ".E0603." "" { target *-*-* } .-1 }
 }

