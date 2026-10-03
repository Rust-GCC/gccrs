macro_rules! foo {
    { $+ } => { // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
        $(x)(y) // { dg-error "" "" { target *-*-* } }
    }
}

foo!();

fn main() {}

