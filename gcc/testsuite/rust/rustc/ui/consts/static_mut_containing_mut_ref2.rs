// revisions: stock mut_refs

#![cfg_attr(mut_refs, feature(const_mut_refs))]

static mut STDERR_BUFFER_SPACE: u8 = 0;

pub static mut STDERR_BUFFER: () = unsafe { *(&mut STDERR_BUFFER_SPACE) = 42; };
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

