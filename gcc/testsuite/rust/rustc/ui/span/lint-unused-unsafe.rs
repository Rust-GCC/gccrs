// Exercise the unused_unsafe attribute in some positive and negative cases

#![allow(dead_code)]
#![deny(unused_unsafe)]


mod foo {
    extern {
        pub fn bar();
    }
}

fn callback<T, F>(_f: F) -> T where F: FnOnce() -> T { panic!() }
unsafe fn unsf() {}

fn bad1() { unsafe {} }                  // { dg-error "" "" { target *-*-* } }
fn bad2() { unsafe { bad1() } }          // { dg-error "" "" { target *-*-* } }
unsafe fn bad3() { unsafe {} }           // { dg-error "" "" { target *-*-* } }
fn bad4() { unsafe { callback(||{}) } }  // { dg-error "" "" { target *-*-* } }
unsafe fn bad5() { unsafe { unsf() } }   // { dg-error "" "" { target *-*-* } }
fn bad6() {
    unsafe {                             // don't put the warning here
        unsafe {                         // { dg-error "" "" { target *-*-* } }
            unsf()
        }
    }
}
unsafe fn bad7() {
    unsafe {                             // { dg-error "" "" { target *-*-* } }
        unsafe {                         // { dg-error "" "" { target *-*-* } }
            unsf()
        }
    }
}

unsafe fn good0() { unsf() }
fn good1() { unsafe { unsf() } }
fn good2() {
    /* bug uncovered when implementing warning about unused unsafe blocks. Be
       sure that when purity is inherited that the source of the unsafe-ness
       is tracked correctly */
    unsafe {
        unsafe fn what() -> Vec<String> { panic!() }

        callback(|| {
            what();
        });
    }
}

unsafe fn good3() { foo::bar() }
fn good4() { unsafe { foo::bar() } }

#[allow(unused_unsafe)] fn allowed() { unsafe {} }

fn main() {}

