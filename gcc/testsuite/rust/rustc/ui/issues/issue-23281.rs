pub struct Struct;

impl Struct {
    pub fn function(funs: Vec<dyn Fn() -> ()>) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

struct Vec<T> {
    t: T,
}

fn main() {}

