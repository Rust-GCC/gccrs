fn main() {
    let x: &str = 'ab';
// { dg-error "" "" { target *-*-* } .-1 }
    let y: char = 'cd';
// { dg-error "" "" { target *-*-* } .-1 }
    let z = 'ef';
// { dg-error "" "" { target *-*-* } .-1 }

    if x == y {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    if y == z {}  // no error here
    if x == z {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    let a: usize = "";
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

