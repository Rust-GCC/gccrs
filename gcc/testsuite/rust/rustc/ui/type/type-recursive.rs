struct T1 { // { dg-error ".E0072." "" { target *-*-* } }
    foo: isize,
    foolish: T1
}

fn main() { }

