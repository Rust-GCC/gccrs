#![deny(unreachable_patterns)]

fn main() {
    let s: &[bool] = &[];

    match s {
        [true, ..] => {}
        [true, ..] => {} // { dg-error "" "" { target *-*-* } }
        [true] => {} // { dg-error "" "" { target *-*-* } }
        [..] => {}
    }
    match s {
        [.., true] => {}
        [.., true] => {} // { dg-error "" "" { target *-*-* } }
        [true] => {} // { dg-error "" "" { target *-*-* } }
        [..] => {}
    }
    match s {
        [false, .., true] => {}
        [false, .., true] => {} // { dg-error "" "" { target *-*-* } }
        [false, true] => {} // { dg-error "" "" { target *-*-* } }
        [false] => {}
        [..] => {}
    }
}

