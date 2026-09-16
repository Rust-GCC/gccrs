// check-pass
// run-rustfix

#![warn(ellipsis_inclusive_range_patterns)]

fn main() {
    let despondency = 2;
    match despondency {
        1...2 => {}
// { dg-warning "" "" { target *-*-* } .-1 }
        _ => {}
    }

    match &despondency {
        &1...2 => {}
// { dg-warning "" "" { target *-*-* } .-1 }
        _ => {}
    }
}

