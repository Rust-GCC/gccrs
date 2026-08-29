pub static mut A: u32 = 0;
pub static mut B: () = unsafe { A = 1; };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

pub static mut C: u32 = unsafe { C = 1; 0 };
// { dg-error ".E0391." "" { target *-*-* } .-1 }

pub static D: u32 = D;

fn main() {}

