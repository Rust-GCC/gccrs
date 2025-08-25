#![crate_type="rlib"]
#![optimize(speed)] // { dg-error ".E0658." "" { target *-*-* } }

#[optimize(size)] // { dg-error ".E0658." "" { target *-*-* } }
mod module {

#[optimize(size)] // { dg-error ".E0658." "" { target *-*-* } }
fn size() {}

#[optimize(speed)] // { dg-error ".E0658." "" { target *-*-* } }
fn speed() {}

#[optimize(banana)]
// { dg-error ".E0722." "" { target *-*-* } .-1 }
// { dg-error ".E0722." "" { target *-*-* } .-2 }
fn not_known() {}

}

