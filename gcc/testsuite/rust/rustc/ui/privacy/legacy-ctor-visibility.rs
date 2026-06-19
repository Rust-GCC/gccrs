use m::S;

mod m {
    pub struct S(u8);

    mod n {
        use S;
        fn f() {
            S(10);
// { dg-error ".E0423." "" { target *-*-* } .-1 }
        }
    }
}

fn main() {}

