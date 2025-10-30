// run-pass
// only-32bit too impatient for 2⁶⁴ items
// compile-flags: -C debug_assertions=no -C opt-level=3

use std::panic;
use std::usize::MAX;

fn main() {
    let n = MAX as u64;
    assert_eq!((0..).by_ref().position(|i| i >= n), Some(MAX));
    assert_eq!((0..).by_ref().position(|i| i > n), Some(0));
    assert_eq!((0..=n + 1).by_ref().position(|_| false), None);
}

