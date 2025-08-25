#![feature(const_fn)]

fn main() {}

#[repr(C)]
union Foo {
    u: u32,
    i: i32,
}

const unsafe fn foo(u: u32) -> i32 {
    Foo { u }.i // { dg-error ".E0658." "" { target *-*-* } }
}

