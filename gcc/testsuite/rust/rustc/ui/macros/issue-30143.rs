use std::fmt::Write;

fn main() {
    println!(0);
// { dg-error "" "" { target *-*-* } .-1 }
    eprintln!('a');
// { dg-error "" "" { target *-*-* } .-1 }
    let mut s = String::new();
    writeln!(s, true).unwrap();
// { dg-error "" "" { target *-*-* } .-1 }
}

