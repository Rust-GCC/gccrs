use std::ops::Add;

trait BrokenAdd: Copy + Add<Output=Self> {
    fn broken_add<T>(&self, rhs: T) -> Self {
        *self + rhs // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
    }
}

impl<T: Copy + Add<Output=T>> BrokenAdd for T {}

pub fn main() {
    let foo: u8 = 0;
    let x: u8 = foo.broken_add("hello darkness my old friend".to_string());
    println!("{}", x);
}

