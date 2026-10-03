fn test() {
    let b = Box::new(1); // { dg-note "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }
    drop(b);
    b = Box::new(2); // { dg-error ".E0384." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
    drop(b);
}

fn main() {
}

