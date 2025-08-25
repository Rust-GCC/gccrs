fn test() {
    let v: isize;
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
    v = 1; // { dg-note "" "" { target *-*-* } }
    println!("v={}", v);
    v = 2; // { dg-error ".E0384." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
    println!("v={}", v);
}

fn main() {
}

