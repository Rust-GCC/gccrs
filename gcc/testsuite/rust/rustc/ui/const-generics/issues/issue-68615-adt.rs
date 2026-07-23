// [full] check-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct Const<const V: [usize; 0]> {}
// { dg-error "" "" { target *-*-* } .-1 }
type MyConst = Const<{ [] }>;

fn main() {
    let _x = Const::<{ [] }> {};
    let _y = MyConst {};
}

