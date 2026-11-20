fn with_int(f: &mut dyn FnMut(&isize)) {}

fn main() {
    let mut x: Option<&isize> = None;
    with_int(&mut |y| x = Some(y));
// { dg-error ".E0521." "" { target *-*-* } .-1 }
}

