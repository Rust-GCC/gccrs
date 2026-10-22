// run-rustfix

fn main() {
    println!('●●'); // { dg-error "" "" { target *-*-* } }
}

