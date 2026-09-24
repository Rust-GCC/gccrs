// Test that NLL analysis propagates lifetimes correctly through
// field accesses, Box accesses, etc.

#![feature(nll)]

fn foo(s: &mut (i32,)) -> i32 {
    let t = &mut *s; // this borrow should last for the entire function
    let x = &t.0;
    *s = (2,); // { dg-error ".E0506." "" { target *-*-* } }
    *x
}

fn bar(s: &Box<(i32,)>) -> &'static i32 {
    // FIXME(#46983): error message should be better
    &s.0 // { dg-error ".E0621." "" { target *-*-* } }
}

fn main() {
    foo(&mut (0,));
    bar(&Box::new((1,)));
}

