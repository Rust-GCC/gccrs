use std::ptr::NonNull;

fn main() {
    let x: &'static NonNull<u32> = &(NonNull::dangling());
// { dg-error ".E0716." "" { target *-*-* } .-1 }

    let mut i: i32 = 10;
    let non_null = NonNull::new(&mut i).unwrap();
    let x: &'static NonNull<u32> = &(non_null.cast());
// { dg-error ".E0716." "" { target *-*-* } .-1 }
}

