// Test that attempt to move `&mut` pointer while pointee is borrowed
// yields an error.
//
// Example from compiler/rustc_borrowck/borrowck/README.md



fn foo(t0: &mut isize) {
    let p: &isize = &*t0; // Freezes `*t0`
    let t1 = t0;        // { dg-error ".E0505." "" { target *-*-* } }
    *t1 = 22;
    p.use_ref();
}

fn main() {
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

