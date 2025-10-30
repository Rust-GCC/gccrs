// run-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

use std::fmt;

struct Array<T>(T);

impl<T: fmt::Debug, const N: usize> fmt::Debug for Array<[T; N]> {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        f.debug_list().entries((&self.0 as &[T]).iter()).finish()
    }
}

fn main() {
    assert_eq!(format!("{:?}", Array([1, 2, 3])), "[1, 2, 3]");
}

