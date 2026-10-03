// Test that the parser is error correcting missing idents. Despite a parsing
// error (or two), we still run type checking (and don't get extra errors there).

fn main() {
    let y = 42;
    let x = y.;  // { dg-error "" "" { target *-*-* } }
    let x = y.();  // { dg-error ".E0618." "" { target *-*-* } }
// { dg-error ".E0618." "" { target *-*-* } .-1 }
    let x = y.foo; // { dg-error ".E0610." "" { target *-*-* } }
}

