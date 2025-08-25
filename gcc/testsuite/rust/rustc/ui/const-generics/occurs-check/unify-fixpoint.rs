#![feature(const_generics)] // { dg-warning "" "" { target *-*-* } }

// It depends on how we normalize constants and how const equate works if this
// compiles.
//
// Please ping @lcnr if the output if this test changes.


fn bind<const N: usize>(value: [u8; N + 2]) -> [u8; N * 2] {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    todo!()
}

fn main() {
    let mut arr = Default::default();
    arr = bind::<2>(arr);
}

