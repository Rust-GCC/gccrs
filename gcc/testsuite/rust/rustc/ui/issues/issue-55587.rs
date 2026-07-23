use std::path::Path;

fn main() {
    let Path::new(); // { dg-error ".E0164." "" { target *-*-* } }
}

