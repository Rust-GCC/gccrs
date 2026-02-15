fn f(y: Box<isize>) {
    *y = 5; // { dg-error ".E0594." "" { target *-*-* } }
}

fn g() {
    let _frob = |q: Box<isize>| { *q = 2; }; // { dg-error ".E0594." "" { target *-*-* } }
}

fn main() {}

