// Test coercions between pointers which don't do anything fancy like unsizing.
// These are testing that we don't lose mutability when converting to raw pointers.

pub fn main() {
    // *const -> *mut
    let x: *const isize = &42;
    let x: *mut isize = x; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }

    // & -> *mut
    let x: *mut isize = &42; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }

    let x: *const isize = &42;
    let x: *mut isize = x; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
}

