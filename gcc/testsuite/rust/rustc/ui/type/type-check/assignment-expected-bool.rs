// The purpose of this text is to ensure that we get good
// diagnostics when a `bool` is expected but that due to
// an assignment expression `x = y` the type is `()`.

fn main() {
    let _: bool = 0 = 0; // { dg-error ".E0308." "" { target *-*-* } }

    let _: bool = match 0 {
        0 => 0 = 0, // { dg-error ".E0308." "" { target *-*-* } }
        _ => 0 = 0, // { dg-error ".E0308." "" { target *-*-* } }
    };

    let _: bool = match true {
        true => 0 = 0, // { dg-error ".E0308." "" { target *-*-* } }
        _ => (),
    };

    if 0 = 0 {} // { dg-error ".E0308." "" { target *-*-* } }

    let _: bool = if { 0 = 0 } { // { dg-error ".E0308." "" { target *-*-* } }
        0 = 0 // { dg-error ".E0308." "" { target *-*-* } }
    } else {
        0 = 0 // { dg-error ".E0308." "" { target *-*-* } }
    };

    let _ = (0 = 0) // { dg-error ".E0308." "" { target *-*-* } }
        && { 0 = 0 } // { dg-error ".E0308." "" { target *-*-* } }
        || (0 = 0); // { dg-error ".E0308." "" { target *-*-* } }

    // A test to check that not expecting `bool` behaves well:
    let _: usize = 0 = 0;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

