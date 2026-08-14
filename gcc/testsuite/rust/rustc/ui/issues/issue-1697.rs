// Testing that we don't fail abnormally after hitting the errors

use unresolved::*; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }

fn main() {}

