// compile-flags: -Z unpretty=mir
// ignore-cloudabi no std::path

use std::path::MAIN_SEPARATOR;

fn main() {
    let mut foo : String = "hello".to_string();
    foo.push(MAIN_SEPARATOR);
    println!("{}", foo);
    let x: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

