fn main() {
    let message = "world";
    println!("Hello, {}", message/); // { dg-error "" "" { target *-*-* } }
}

