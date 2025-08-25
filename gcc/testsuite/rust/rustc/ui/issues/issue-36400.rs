fn f(x: &mut u32) {}

fn main() {
    let x = Box::new(3);
    f(&mut *x); // { dg-error ".E0596." "" { target *-*-* } }
}

