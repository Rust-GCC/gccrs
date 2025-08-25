enum X {
    Y(u32)
}

fn main() {
    match X::Y(0) {
        X::Y { number } => {}
// { dg-error ".E0769." "" { target *-*-* } .-1 }
    }
}

