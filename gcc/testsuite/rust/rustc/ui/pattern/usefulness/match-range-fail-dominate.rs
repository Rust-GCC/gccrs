#![deny(unreachable_patterns, overlapping_patterns)]

fn main() {
    match 5 {
      1 ..= 10 => { }
      5 ..= 6 => { }
// { dg-error "" "" { target *-*-* } .-1 }
      _ => {}
    };

    match 5 {
      3 ..= 6 => { }
      4 ..= 6 => { }
// { dg-error "" "" { target *-*-* } .-1 }
      _ => {}
    };

    match 5 {
      4 ..= 6 => { }
      4 ..= 6 => { }
// { dg-error "" "" { target *-*-* } .-1 }
      _ => {}
    };

    match 'c' {
      'A' ..= 'z' => {}
      'a' ..= 'z' => {}
// { dg-error "" "" { target *-*-* } .-1 }
      _ => {}
    };

    match 1.0f64 {
      0.01f64 ..= 6.5f64 => {}
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
// { dg-warning "" "" { target *-*-* } .-5 }
// { dg-warning "" "" { target *-*-* } .-6 }
// { dg-warning "" "" { target *-*-* } .-7 }
// { dg-warning "" "" { target *-*-* } .-8 }
      0.02f64 => {} // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
      _ => {}
    };
}

