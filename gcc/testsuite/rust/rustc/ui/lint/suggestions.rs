// ignore-tidy-tab
// run-rustfix

#![warn(unused_mut, unused_parens)] // UI tests pass `-A unused`—see Issue #43896

#[no_mangle] const DISCOVERY: usize = 1;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

#[no_mangle]
// { help "" "" { target *-*-* } .-1 }
pub fn defiant<T>(_t: T) {}
// { dg-warning "" "" { target *-*-* } .-1 }

#[no_mangle]
fn rio_grande() {}

mod badlands {
    // The private-no-mangle lints shouldn't suggest inserting `pub` when the
    // item is already `pub` (but triggered the lint because, e.g., it's in a
    // private module). (Issue #47383)
    #[no_mangle] pub const DAUNTLESS: bool = true;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    #[allow(dead_code)] // for rustfix
    #[no_mangle] pub fn val_jean<T>() {}
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

    // ... but we can suggest just-`pub` instead of restricted
    #[no_mangle] pub(crate) const VETAR: bool = true;
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
    #[allow(dead_code)] // for rustfix
    #[no_mangle] pub(crate) fn crossfield<T>() {}
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
}

struct Equinox {
    warp_factor: f32,
}

fn main() {
    while true {
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
        let mut registry_no = (format!("NX-{}", 74205));
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { help "" "" { target *-*-* } .-4 }
        // the line after `mut` has a `\t` at the beginning, this is on purpose
        let mut
	        b = 1;
// { dg-warning "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-2 }
        let d = Equinox { warp_factor: 9.975 };
        match d {
            #[allow(unused_variables)] // for rustfix
            Equinox { warp_factor: warp_factor } => {}
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
        }
        println!("{} {}", registry_no, b);
    }
}

