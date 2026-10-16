const C1: &'static mut [usize] = &mut [];
// { dg-error ".E0764." "" { target *-*-* } .-1 }

static mut S: usize = 3;
const C2: &'static mut usize = unsafe { &mut S };
// { dg-error ".E0764." "" { target *-*-* } .-1 }
// { dg-error ".E0764." "" { target *-*-* } .-2 }
// { dg-error ".E0764." "" { target *-*-* } .-3 }

fn main() {}

