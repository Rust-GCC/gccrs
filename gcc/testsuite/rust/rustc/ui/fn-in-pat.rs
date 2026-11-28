struct A {}

impl A {
    fn new() {}
}

fn hof<F>(_: F) where F: FnMut(()) {}

fn ice() {
    hof(|c| match c {
        A::new() => (), // { dg-error ".E0164." "" { target *-*-* } }
        _ => ()
    })
}

fn main() {}

