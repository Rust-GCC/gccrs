// ignore-tidy-linelength

fn main() {
    println!(“hello world”);
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { help "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-5 }
}

