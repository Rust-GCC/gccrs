// Check that dynamically sized rvalues are forbidden

#![feature(box_syntax)]

pub fn main() {
    let _x: Box<str> = box *"hello world";
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { dg-error ".E0507." "" { target *-*-* } .-2 }

    let array: &[isize] = &[1, 2, 3];
    let _x: Box<[isize]> = box *array;
// { dg-error ".E0508." "" { target *-*-* } .-1 }
// { dg-error ".E0508." "" { target *-*-* } .-2 }
}

