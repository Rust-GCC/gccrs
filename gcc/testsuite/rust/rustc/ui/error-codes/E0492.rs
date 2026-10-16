use std::sync::atomic::AtomicUsize;

const A: AtomicUsize = AtomicUsize::new(0);
static B: &'static AtomicUsize = &A; // { dg-error ".E0492." "" { target *-*-* } }

fn main() {
}

