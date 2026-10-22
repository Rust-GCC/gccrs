fn main() {
    let _vec: Vec<&'static String> = vec![&String::new()];
// { dg-error ".E0716." "" { target *-*-* } .-1 }
}

