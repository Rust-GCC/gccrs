fn main() {
    let r: f32 = 1.0;
    match r {
        [a, b] => {
// { dg-error ".E0529." "" { target *-*-* } .-1 }
        }
    }
}

