// Regression test for issue #5239

fn main() {
    let x = |ref x: isize| { x += 1; };
// { dg-error ".E0368." "" { target *-*-* } .-1 }
}

