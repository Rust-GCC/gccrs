// compile-flags: -Z unpretty=mir

fn main() {
    let x: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

