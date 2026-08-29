// Issue #18317

mod bleh {
    macro_rules! defn {
        ($n:ident) => (
            fn $n (&self) -> i32 {
                println!("{}", stringify!($n));
                1
            }
        )
    }

    #[derive(Copy, Clone)]
    pub struct S;

    impl S {
        pub defn!(f); // { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    }
}

fn main() {}

