// This test originated from #65394. We conservatively assume that `x` is still `LiveDrop` even
// after it has been moved because a mutable reference to it exists at some point in the const body.
//
// We will likely have to change this behavior before we allow `&mut` in a `const`.

const _: Vec<i32> = {
    let mut x = Vec::<i32>::new(); // { dg-error ".E0493." "" { target *-*-* } }
    let r = &mut x; // { dg-error ".E0764." "" { target *-*-* } }
    let y = x;
    y
};

fn main() {}

