fn main() {
    let a = if true {
        0
    } else if false {
// { dg-error ".E0317." "" { target *-*-* } .-1 }
// { dg-error ".E0317." "" { target *-*-* } .-2 }
        1
    };
}

