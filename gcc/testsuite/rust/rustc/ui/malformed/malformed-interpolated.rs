#![feature(rustc_attrs)]

macro_rules! check {
    ($expr: expr) => (
        #[rustc_dummy = $expr] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
        use main as _;
    );
}

check!("0"); // OK
check!(0); // OK
check!(0u8); // { dg-error "" "" { target *-*-* } }
check!(-0); // ERROR, see above
check!(0 + 0); // ERROR, see above

fn main() {}

