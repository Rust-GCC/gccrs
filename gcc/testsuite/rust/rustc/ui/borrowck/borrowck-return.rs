fn f() -> isize {
    let x: isize;
    return x; // { dg-error ".E0381." "" { target *-*-* } }
}

fn main() { f(); }

