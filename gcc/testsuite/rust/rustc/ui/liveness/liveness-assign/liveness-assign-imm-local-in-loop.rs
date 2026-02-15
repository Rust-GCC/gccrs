fn test() {
    let v: isize;
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
    loop {
        v = 1; // { dg-error ".E0384." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
    }
}

fn main() {
}

