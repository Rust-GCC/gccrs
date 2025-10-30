#![feature(generators, generator_trait, box_leak)]

use std::cell::RefCell;
use std::ops::Generator;
use std::pin::Pin;

fn main() {
    let (mut gen, cell);
    cell = Box::new(RefCell::new(0));
    let ref_ = Box::leak(Box::new(Some(cell.borrow_mut())));
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    // the upvar is the non-dropck `&mut Option<Ref<'a, i32>>`.
    gen = || {
        // but the generator can use it to drop a `Ref<'a, i32>`.
        let _d = ref_.take(); // { dg-error ".E0597." "" { target *-*-* } }
        yield;
    };
    Pin::new(&mut gen).resume(());
    // drops the RefCell and then the Ref, leading to use-after-free
}

