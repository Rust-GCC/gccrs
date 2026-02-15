fn a<F:Fn(isize, isize) -> isize>(mut f: F) {
    let g = &mut f;
    f(1, 2);    // { dg-error ".E0502." "" { target *-*-* } }
    use_mut(g);
}
fn b<F:FnMut(isize, isize) -> isize>(f: F) {
    f(1, 2);    // { dg-error ".E0596." "" { target *-*-* } }
}

fn c<F:FnOnce(isize, isize) -> isize>(f: F) {
    f(1, 2);
    f(1, 2);    // { dg-error ".E0382." "" { target *-*-* } }
}

fn main() {}

fn use_mut<T>(_: &mut T) { }

