#![deny(unreachable_patterns)]

fn a() {
    let v = [1, 2, 3];
    match v {
        [_, _, _] => {}
        [_, _, _] => {} // { dg-error "" "" { target *-*-* } }
    }
    match v {
        [_, 1, _] => {}
        [_, 1, _] => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
}

fn main() {
    a();
}

