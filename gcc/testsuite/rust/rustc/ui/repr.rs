#[repr] // { dg-error "" "" { target *-*-* } }
struct _A {}

#[repr = "B"] // { dg-error "" "" { target *-*-* } }
struct _B {}

#[repr = "C"] // { dg-error "" "" { target *-*-* } }
struct _C {}

#[repr(C)]
struct _D {}

fn main() {}

