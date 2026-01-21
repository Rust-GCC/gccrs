// Companion test to the similarly-named file in run-pass.

// compile-flags: -C debug_assertions=yes
// revisions: std core

#![feature(lang_items)]
#![cfg_attr(core, no_std)]

#[cfg(std)] use std::fmt;
#[cfg(core)] use core::fmt;
#[cfg(core)] #[lang = "eh_personality"] fn eh_personality() {}
#[cfg(core)] #[lang = "eh_catch_typeinfo"] static EH_CATCH_TYPEINFO: u8 = 0;
#[cfg(core)] #[lang = "panic_impl"] fn panic_impl(panic: &core::panic::PanicInfo) -> ! { loop {} }

// (see documentation of the similarly-named test in run-pass)
fn to_format_or_not_to_format() {
    let falsum = || false;

    // assert!(true, "{}",); // see run-pass

    assert_eq!(1, 1, "{}",);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    assert_ne!(1, 2, "{}",);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    // debug_assert!(true, "{}",); // see run-pass

    debug_assert_eq!(1, 1, "{}",);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    debug_assert_ne!(1, 2, "{}",);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    #[cfg(std)] {
        eprint!("{}",);
// { dg-error "" "" { target *-*-* } .-1 }
    }

    #[cfg(std)] {
        // FIXME: compile-fail says "expected error not found" even though
        //        rustc does emit an error
        // eprintln!("{}",);
        // <DISABLED> [std]~^ ERROR no arguments
    }

    #[cfg(std)] {
        format!("{}",);
// { dg-error "" "" { target *-*-* } .-1 }
    }

    format_args!("{}",);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    // if falsum() { panic!("{}",); } // see run-pass

    #[cfg(std)] {
        print!("{}",);
// { dg-error "" "" { target *-*-* } .-1 }
    }

    #[cfg(std)] {
        // FIXME: compile-fail says "expected error not found" even though
        //        rustc does emit an error
        // println!("{}",);
        // <DISABLED> [std]~^ ERROR no arguments
    }

    unimplemented!("{}",);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    // if falsum() { unreachable!("{}",); } // see run-pass

    struct S;
    impl fmt::Display for S {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
            write!(f, "{}",)?;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

            // FIXME: compile-fail says "expected error not found" even though
            //        rustc does emit an error
            // writeln!(f, "{}",)?;
            // <DISABLED> [core]~^ ERROR no arguments
            // <DISABLED> [std]~^^ ERROR no arguments
            Ok(())
        }
    }
}

fn main() {}

