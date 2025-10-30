//compile-flags: -Zborrowck=mir

#![allow(warnings)]

struct Wrap<'p> { p: &'p mut i32 }

impl<'p> Drop for Wrap<'p> {
    fn drop(&mut self) {
        *self.p += 1;
    }
}

fn main() {
    let mut x = 0;
    let wrap = Wrap { p: &mut x };
    x = 1; // { dg-error ".E0506." "" { target *-*-* } }
}

