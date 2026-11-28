// run-rustfix

fn main() {
    for _i 0..2 { // { dg-error "" "" { target *-*-* } }
    }
    for _i of 0..2 { // { dg-error "" "" { target *-*-* } }
    }
}

