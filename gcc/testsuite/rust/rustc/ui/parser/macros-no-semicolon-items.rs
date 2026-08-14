macro_rules! foo()  // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

macro_rules! bar {
    ($($tokens:tt)*) => {}
}

bar!( // { dg-error "" "" { target *-*-* } }
    blah
    blah
    blah
)

fn main() {
}

