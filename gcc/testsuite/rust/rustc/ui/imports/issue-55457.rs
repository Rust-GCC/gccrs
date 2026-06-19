use NonExistent; // { dg-error ".E0432." "" { target *-*-* } }
use non_existent::non_existent; // { dg-error ".E0432." "" { target *-*-* } }

#[non_existent] // { dg-error "" "" { target *-*-* } }
#[derive(NonExistent)] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
struct S;

fn main() {}

