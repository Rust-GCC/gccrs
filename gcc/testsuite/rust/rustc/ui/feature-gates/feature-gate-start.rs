#[start]
fn foo(_: isize, _: *const *const u8) -> isize { 0 }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

