//[full] run-pass
// Verifies that having generic parameters after constants is not permitted without the
// `const_generics` feature.
// revisions: none min full

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct A<const N: usize, T=u32>(T);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }

fn main() {
  let _: A<3> = A(0);
}

