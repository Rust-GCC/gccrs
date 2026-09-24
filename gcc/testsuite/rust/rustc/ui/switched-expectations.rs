fn main() {
    let var = 10i32;
    let ref string: String = var; // { dg-error ".E0308." "" { target *-*-* } }
}

