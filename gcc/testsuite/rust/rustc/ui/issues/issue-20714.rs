struct G;

fn main() {
    let g = G(); // { dg-error ".E0618." "" { target *-*-* } }
}

