fn test() {
    let v: isize;
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
    v = 2;  // { dg-note "" "" { target *-*-* } }
    v += 1; // { dg-error ".E0384." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
    v.clone();
}

fn main() {
}

