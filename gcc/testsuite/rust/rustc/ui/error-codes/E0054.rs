fn main() {
    let x = 5;
    let x_is_nonzero = x as bool; // { dg-error ".E0054." "" { target *-*-* } }
}

