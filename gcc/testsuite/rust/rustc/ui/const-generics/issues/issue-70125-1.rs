// run-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

const L: usize = 4;

pub trait Print<const N: usize> {
    fn print(&self) -> usize {
        N
    }
}

pub struct Printer;
impl Print<L> for Printer {}

fn main() {
    let p = Printer;
    assert_eq!(p.print(), 4);
}

