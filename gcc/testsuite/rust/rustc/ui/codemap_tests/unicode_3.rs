// check-pass

fn main() {
    let s = "ZͨA͑ͦ͒͋ͤ͑̚L̄͑͋Ĝͨͥ̿͒̽̈́Oͥ͛ͭ!̏"; while true { break; } // { dg-warning "" "" { target *-*-* } }
    println!("{}", s);
}

