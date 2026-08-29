fn main() {
    println!(""); // { dg-error "" "" { target *-*-* } }
    let x: usize = (); // { dg-error ".E0308." "" { target *-*-* } }
}

