// Regression test for issue #61033.

macro_rules! test1 {
    ($x:ident, $($tt:tt)*) => { $($tt)+ } // { dg-error "" "" { target *-*-* } }
}

fn main() {
    test1!(x,);
    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

