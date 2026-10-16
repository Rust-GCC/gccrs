fn main() {
    let _: Vec<&str, "a"> = Vec::new(); // { dg-error ".E0107." "" { target *-*-* } }
}

