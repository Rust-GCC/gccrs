// run-rustfix

fn main() {
    let _x = ~1; // { dg-error "" "" { target *-*-* } }
}

