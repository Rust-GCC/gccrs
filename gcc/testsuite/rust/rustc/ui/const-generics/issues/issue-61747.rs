// revisions: full min
#![cfg_attr(full, feature(const_generics))] // { dg-warning "" "" { target *-*-* } }
#![cfg_attr(min, feature(min_const_generics))]

struct Const<const N: usize>;

impl<const C: usize> Const<{C}> {
    fn successor() -> Const<{C + 1}> {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        Const
    }
}

fn main() {
    let _x: Const::<2> = Const::<1>::successor();
}

