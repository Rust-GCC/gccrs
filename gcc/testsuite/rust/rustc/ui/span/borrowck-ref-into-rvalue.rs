fn main() {
    let msg;
    match Some("Hello".to_string()) {
// { dg-error ".E0716." "" { target *-*-* } .-1 }
        Some(ref m) => {
            msg = m;
        },
        None => { panic!() }
    }
    println!("{}", *msg);
}

