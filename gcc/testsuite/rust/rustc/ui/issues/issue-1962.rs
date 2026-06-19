// compile-flags: -D while-true
// run-rustfix

fn main() {
    let mut i = 0;
    while true { // { dg-error "" "" { target *-*-* } }
        i += 1;
        if i == 5 { break; }
    }
}

