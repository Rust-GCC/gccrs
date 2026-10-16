#![deny(nonstandard_style)]
#![allow(dead_code)]

fn CamelCase() {} // { dg-error "" "" { target *-*-* } }

#[allow(nonstandard_style)]
mod test {
    fn CamelCase() {}

    #[forbid(nonstandard_style)]
    mod bad {
        fn CamelCase() {} // { dg-error "" "" { target *-*-* } }

        static bad: isize = 1; // { dg-error "" "" { target *-*-* } }
    }

    mod warn {
        #![warn(nonstandard_style)]

        fn CamelCase() {} // { dg-warning "" "" { target *-*-* } }

        struct snake_case; // { dg-warning "" "" { target *-*-* } }
    }
}

fn main() {}

