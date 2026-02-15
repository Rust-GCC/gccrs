// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

fn foo<const LEN: usize, const DATA: [u8; LEN]>() {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
fn main() {
    const DATA: [u8; 4] = *b"ABCD";
    foo::<4, DATA>();
// { dg-error "" "" { target *-*-* } .-1 }
}

