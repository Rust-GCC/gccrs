// Ensures #[unstable] functions without opting in the corresponding #![feature]
// will not break inference.

// aux-build:inference_unstable_iterator.rs
// aux-build:inference_unstable_itertools.rs
// run-pass

extern crate inference_unstable_iterator;
extern crate inference_unstable_itertools;

#[allow(unused_imports)]
use inference_unstable_iterator::IpuIterator;
use inference_unstable_itertools::IpuItertools;

fn main() {
    assert_eq!('x'.ipu_flatten(), 1);
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

