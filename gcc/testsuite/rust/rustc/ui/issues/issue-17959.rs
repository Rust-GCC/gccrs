extern crate core;

use core::ops::Drop;

trait Bar {}

struct G<T: ?Sized> {
    _ptr: *const T
}

impl<T> Drop for G<T> {
// { dg-error ".E0367." "" { target *-*-* } .-1 }
    fn drop(&mut self) {
        if !self._ptr.is_null() {
        }
    }
}

fn main() {
    let x:G<dyn Bar>;
}

