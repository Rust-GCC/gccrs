// Semantically, a free `const` item cannot omit its body.

fn main() {}

const A: u8; // { dg-error "" "" { target *-*-* } }
const B; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }

