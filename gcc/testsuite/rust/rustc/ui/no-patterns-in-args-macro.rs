macro_rules! m {
    ($pat: pat) => {
        trait Tr {
            fn trait_method($pat: u8);
        }

        type A = fn($pat: u8);

        extern "C" {
            fn foreign_fn($pat: u8);
        }
    };
}

mod good_pat {
    m!(good_pat); // OK
}

mod bad_pat {
    m!((bad, pat));
// { dg-error ".E0130." "" { target *-*-* } .-1 }
// { dg-error ".E0130." "" { target *-*-* } .-2 }
// { dg-error ".E0130." "" { target *-*-* } .-3 }
}

fn main() {}

