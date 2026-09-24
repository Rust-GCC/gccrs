fn main() {
    let u = 5 as bool; // { dg-error ".E0054." "" { target *-*-* } }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }
    let t = (1 + 2) as bool; // { dg-error ".E0054." "" { target *-*-* } }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }
    let v = "hello" as bool; // { dg-error ".E0606." "" { target *-*-* } }
}

