// Semantically, a free `static` item cannot omit its body.

fn main() {}

static A: u8; // { dg-error "" "" { target *-*-* } }
static B; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }

static mut C: u8; // { dg-error "" "" { target *-*-* } }
static mut D; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }

