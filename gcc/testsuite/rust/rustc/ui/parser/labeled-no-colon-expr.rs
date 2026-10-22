#![feature(label_break_value)]

fn main() {
    'l0 while false {} // { dg-error "" "" { target *-*-* } }
    'l1 for _ in 0..1 {} // { dg-error "" "" { target *-*-* } }
    'l2 loop {} // { dg-error "" "" { target *-*-* } }
    'l3 {} // { dg-error "" "" { target *-*-* } }
    'l4 0; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }

    macro_rules! m {
        ($b:block) => {
            'l5 $b; // { dg-error "" "" { target *-*-* } }
        }
    }
    m!({}); // { dg-error "" "" { target *-*-* } }
}

