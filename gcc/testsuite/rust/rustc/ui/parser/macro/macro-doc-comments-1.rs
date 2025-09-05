macro_rules! outer {
    (#[$outer:meta]) => ()
}

outer! {
    //! Inner
// { dg-error "" "" { target *-*-* } .-1 }

fn main() { }

