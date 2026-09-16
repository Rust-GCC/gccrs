// This test checks that all expected errors occur when there are multiple invalid attributes
// on an item.

#[inline]
// { dg-error ".E0518." "" { target *-*-* } .-1 }
#[target_feature(enable = "sse2")]
// { dg-error "" "" { target *-*-* } .-1 }
const FOO: u8 = 0;

fn main() { }

