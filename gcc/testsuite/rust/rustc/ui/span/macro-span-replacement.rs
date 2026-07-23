// check-pass

#![warn(unused)]

macro_rules! m {
    ($a:tt $b:tt) => {
        $b $a; // { dg-warning "" "" { target *-*-* } }
    }
}

fn main() {
    m!(S struct);
}

