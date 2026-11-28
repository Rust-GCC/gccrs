// Issue #11669

// ignore-tidy-cr

fn main() {
    // \r\n
    let ok = "This is \
 a test";
    // \r only
    let bad = "This is \ a test";
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

}

