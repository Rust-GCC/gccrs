fn main() {
    let x = 0;
    match 1 {
        0 ..= x => {}
// { dg-error ".E0080." "" { target *-*-* } .-1 }
    };
}

