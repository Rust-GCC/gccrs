#![feature(box_into_boxed_slice)]

use std::boxed::Box;
use std::fmt::Debug;
fn main() {
    let boxed_slice = Box::new([1,2,3]) as Box<[u8]>;
    let _ = Box::into_boxed_slice(boxed_slice);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
    let boxed_trait: Box<dyn Debug> = Box::new(5u8);
    let _ = Box::into_boxed_slice(boxed_trait);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

