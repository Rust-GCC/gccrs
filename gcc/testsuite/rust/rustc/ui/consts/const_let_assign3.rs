#![feature(const_fn)]

struct S {
    state: u32,
}

impl S {
    const fn foo(&mut self, x: u32) {
// { dg-error ".E0658." "" { target *-*-* } .-1 }
        self.state = x;
    }
}

const FOO: S = {
    let mut s = S { state: 42 };
    s.foo(3); // { dg-error ".E0764." "" { target *-*-* } }
    s
};

type Array = [u32; {
    let mut x = 2;
    let y = &mut x; // { dg-error ".E0764." "" { target *-*-* } }
    *y = 42;
    *y
}];

fn main() {
    assert_eq!(FOO.state, 3);
}

