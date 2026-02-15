fn with_int<F>(f: F)
where
    F: FnOnce(&isize),
{
    let x = 3;
    f(&x);
}

fn main() {
    let mut x = None;
    with_int(|y| x = Some(y));
// { dg-error ".E0521." "" { target *-*-* } .-1 }
}

