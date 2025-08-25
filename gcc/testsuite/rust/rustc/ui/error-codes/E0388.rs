static X: i32 = 1;
const C: i32 = 2;

const CR: &'static mut i32 = &mut C; // { dg-error ".E0764." "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
static STATIC_REF: &'static mut i32 = &mut X; // { dg-error ".E0596." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

static CONST_REF: &'static mut i32 = &mut C; // { dg-error ".E0764." "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }

fn main() {}

