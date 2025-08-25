fn main() {
    let r = &[1, 2, 3, 4];
    match r {
        &[a, b] => {
// { dg-error ".E0527." "" { target *-*-* } .-1 }
            println!("a={}, b={}", a, b);
        }
    }
}

