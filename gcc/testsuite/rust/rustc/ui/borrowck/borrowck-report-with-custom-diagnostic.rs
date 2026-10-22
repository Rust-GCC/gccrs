#![feature(rustc_attrs)]
#![allow(dead_code)]
fn main() { #![rustc_error] // rust-lang/rust#49855
    // Original borrow ends at end of function
    let mut x = 1;
    let y = &mut x;
// { dg-error "" "" { target *-*-* } .-1 }
    let z = &x; // { dg-error ".E0502." "" { target *-*-* } }
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    z.use_ref();
    y.use_mut();
}

fn foo() {
    match true {
        true => {
            // Original borrow ends at end of match arm
            let mut x = 1;
            let y = &x;
// { dg-error "" "" { target *-*-* } .-1 }
            let z = &mut x; // { dg-error ".E0502." "" { target *-*-* } }
// { dg-error ".E0502." "" { target *-*-* } .-1 }
            z.use_mut();
            y.use_ref();
        }
        false => ()
    }
}

fn bar() {
    // Original borrow ends at end of closure
    || {
        let mut x = 1;
        let y = &mut x;
// { dg-error "" "" { target *-*-* } .-1 }
        let z = &mut x; // { dg-error ".E0499." "" { target *-*-* } }
// { dg-error ".E0499." "" { target *-*-* } .-1 }
        z.use_mut();
        y.use_mut();
    };
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

