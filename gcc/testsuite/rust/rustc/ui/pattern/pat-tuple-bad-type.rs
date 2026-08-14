fn main() {
    let x;

    match x {
        (..) => {} // { dg-error ".E0282." "" { target *-*-* } }
        _ => {}
    }

    match 0u8 {
        (..) => {} // { dg-error ".E0308." "" { target *-*-* } }
        _ => {}
    }

    x = 10;
}

