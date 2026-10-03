fn f(x: usize) -> usize {
    x
}

fn main() {
    let _ = [0; f(2)];
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }
}

