use std::mem;

trait Misc {}

fn size_of_copy<T: Copy+?Sized>() -> usize { mem::size_of::<T>() }

fn main() {
    size_of_copy::<dyn Misc + Copy>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
}

