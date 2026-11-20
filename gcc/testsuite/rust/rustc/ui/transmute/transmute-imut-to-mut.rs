// Tests that transmuting from &T to &mut T is Undefined Behavior.

use std::mem::transmute;

fn main() {
    let _a: &mut u8 = unsafe { transmute(&1u8) };
// { dg-error "" "" { target *-*-* } .-1 }
}

