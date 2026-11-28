fn f() -> isize {
    let mut x: isize;
    while 1 == 1 { x = 10; }
    return x; // { dg-error ".E0381." "" { target *-*-* } }
}

fn main() { f(); }

