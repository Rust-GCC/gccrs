static X: i32 = 1;
const C: i32 = 2;
static mut M: i32 = 3;

const CR: &'static mut i32 = &mut C; // { dg-error ".E0764." "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
static STATIC_REF: &'static mut i32 = &mut X; // { dg-error ".E0596." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

static CONST_REF: &'static mut i32 = &mut C; // { dg-error ".E0764." "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
static STATIC_MUT_REF: &'static mut i32 = unsafe { &mut M }; // { dg-error ".E0764." "" { target *-*-* } }
fn main() {}

