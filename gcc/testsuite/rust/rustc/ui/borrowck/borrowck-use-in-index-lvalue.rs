fn test() {
    let w: &mut [isize];
    w[5] = 0; // { dg-error ".E0381." "" { target *-*-* } }

    let mut w: &mut [isize];
    w[5] = 0; // { dg-error ".E0381." "" { target *-*-* } }
}

fn main() { test(); }

