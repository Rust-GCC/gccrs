fn main() {
    let A = 3;
// { dg-error ".E0005." "" { target *-*-* } .-1 }
// { dg-error ".E0005." "" { target *-*-* } .-2 }
// { help ".E0005." "" { target *-*-* } .-3 }
// { suggestion ".E0005." "" { target *-*-* } .-4 }

    const A: i32 = 2;
// { dg-error "" "" { target *-*-* } .-1 }
}

