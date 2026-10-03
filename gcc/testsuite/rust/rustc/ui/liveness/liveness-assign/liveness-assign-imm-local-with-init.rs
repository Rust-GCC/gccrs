fn test() {
    let v: isize = 1; // { dg-note "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }
    v.clone();
    v = 2; // { dg-error ".E0384." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
    v.clone();
}

fn main() {
}

