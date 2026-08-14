fn main() {
    let a = &[];
    let b: &Vec<u8> = &vec![];
    a > b;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

