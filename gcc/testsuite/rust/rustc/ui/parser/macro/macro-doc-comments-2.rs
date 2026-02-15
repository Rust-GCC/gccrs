macro_rules! inner {
    (#![$inner:meta]) => ()
}

inner! {
    /// Outer
// { dg-error "" "" { target *-*-* } .-1 }

fn main() { }

