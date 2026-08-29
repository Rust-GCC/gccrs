// Test for issue #14581.

fn f((a, a): (isize, isize)) {} // { dg-error ".E0415." "" { target *-*-* } }

fn main() {
    let (a, a) = (1, 1);    // { dg-error ".E0416." "" { target *-*-* } }
}

