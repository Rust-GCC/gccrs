// edition:2018

foo!(); // { dg-error "" "" { target *-*-* } }

pub(in ::bar) struct Baz {} // { dg-error ".E0578." "" { target *-*-* } }

fn main() {}

