// [full] run-pass
// revisions: full min
// Checks a complicated usage of unordered params
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]
#![allow(dead_code)]

struct NestedArrays<'a, const N: usize, A: 'a, const M: usize, T:'a =u32> {
// { dg-error "" "" { target *-*-* } .-1 }
  args: &'a [&'a [T; M]; N],
  specifier: A,
}

fn main() {
  let array = [1, 2, 3];
  let nest = [&array];
  let _ = NestedArrays {
    args: &nest,
    specifier: true,
  };
}

