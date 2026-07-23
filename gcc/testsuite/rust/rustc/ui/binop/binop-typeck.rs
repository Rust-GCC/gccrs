// issue #500

fn main() {
    let x = true;
    let y = 1;
    let z = x + y;
// { dg-error ".E0369." "" { target *-*-* } .-1 }
}

