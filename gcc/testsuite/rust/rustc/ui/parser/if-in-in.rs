// run-rustfix

fn main() {
    for i in in 1..2 { // { dg-error "" "" { target *-*-* } }
        println!("{}", i);
    }
}

