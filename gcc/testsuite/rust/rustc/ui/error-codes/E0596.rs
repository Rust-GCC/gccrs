fn main() {
    let x = 1;
    let y = &mut x; // { dg-error ".E0596." "" { target *-*-* } }
}

