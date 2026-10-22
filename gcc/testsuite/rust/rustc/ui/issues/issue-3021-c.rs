fn siphash<T>() {

    trait U {
        fn g(&self, x: T) -> T;  // { dg-error ".E0401." "" { target *-*-* } }
// { dg-error ".E0401." "" { target *-*-* } .-1 }
    }
}

fn main() {}

