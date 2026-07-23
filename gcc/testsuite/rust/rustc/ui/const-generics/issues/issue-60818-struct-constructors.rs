// check-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))] // { dg-warning "" "" { target *-*-* } }
#![cfg_attr(min, feature(min_const_generics))]

struct Generic<const V: usize>;

fn main() {
    let _ = Generic::<0>;
}

