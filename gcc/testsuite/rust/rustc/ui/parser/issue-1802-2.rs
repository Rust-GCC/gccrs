fn log(a: i32, b: i32) {}

fn main() {
    let error = 42;
    log(error, 0b);
// { dg-error ".E0768." "" { target *-*-* } .-1 }
}

