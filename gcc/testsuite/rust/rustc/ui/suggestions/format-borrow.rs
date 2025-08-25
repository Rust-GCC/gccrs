fn main() {
    let a: String = &String::from("a");
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let b: String = &format!("b");
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

