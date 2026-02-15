// Test that we don't show variables with from for loop desugaring

fn for_loop(s: &[i32]) {
    for &ref mut x in s {}
// { dg-error ".E0596." "" { target *-*-* } .-1 }
}

struct D<'a>(&'a ());

impl Drop for D<'_> {
    fn drop(&mut self) {}
}

fn for_loop_dropck(v: Vec<D<'static>>) {
    for ref mut d in v {
        let y = ();
        *d = D(&y); // { dg-error ".E0597." "" { target *-*-* } }
    }
}

fn main() {}

