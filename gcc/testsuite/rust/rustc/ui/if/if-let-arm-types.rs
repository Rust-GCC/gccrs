fn main() {
    if let Some(b) = None {
// { dg-note ".E0308." "" { target *-*-* } .-1 }
        ()
// { dg-note "" "" { target *-*-* } .-1 }
    } else {
        1
    };
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-2 }
}

