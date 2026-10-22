#[cfg_attr] // { dg-error "" "" { target *-*-* } }
struct S1;

#[cfg_attr = ""] // { dg-error "" "" { target *-*-* } }
struct S2;

#[derive] // { dg-error "" "" { target *-*-* } }
struct S3;

#[derive = ""] // { dg-error "" "" { target *-*-* } }
struct S4;

fn main() {}

