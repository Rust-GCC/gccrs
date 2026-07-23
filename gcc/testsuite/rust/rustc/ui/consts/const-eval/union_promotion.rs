#![allow(const_err)]

#[repr(C)]
union Foo {
    a: &'static u32,
    b: usize,
}

fn main() {
    let x: &'static bool = &unsafe { // { dg-error ".E0716." "" { target *-*-* } }
        Foo { a: &1 }.b == Foo { a: &2 }.b
    };
}

