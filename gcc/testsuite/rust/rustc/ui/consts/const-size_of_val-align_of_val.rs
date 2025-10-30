// run-pass

#![feature(const_size_of_val, const_align_of_val)]

use std::mem;

struct Foo(u32);

#[derive(Clone, Copy)]
struct Bar {
    _x: u8,
    _y: u16,
    _z: u8,
}

union Ugh {
    _a: [u8; 3],
    _b: Bar,
}

const FOO: Foo = Foo(4);
const BAR: Bar = Bar { _x: 4, _y: 1, _z: 2 };
const UGH: Ugh = Ugh { _a: [0; 3] };

const SIZE_OF_FOO: usize = mem::size_of_val(&FOO);
const SIZE_OF_BAR: usize = mem::size_of_val(&BAR);
const SIZE_OF_UGH: usize = mem::size_of_val(&UGH);

const ALIGN_OF_FOO: usize = mem::align_of_val(&FOO);
const ALIGN_OF_BAR: usize = mem::align_of_val(&BAR);
const ALIGN_OF_UGH: usize = mem::align_of_val(&UGH);

const SIZE_OF_SLICE: usize = mem::size_of_val("foobar".as_bytes());

fn main() {
    assert_eq!(SIZE_OF_FOO, mem::size_of::<Foo>());
    assert_eq!(SIZE_OF_BAR, mem::size_of::<Bar>());
    assert_eq!(SIZE_OF_UGH, mem::size_of::<Ugh>());

    assert_eq!(ALIGN_OF_FOO, mem::align_of::<Foo>());
    assert_eq!(ALIGN_OF_BAR, mem::align_of::<Bar>());
    assert_eq!(ALIGN_OF_UGH, mem::align_of::<Ugh>());

    assert_eq!(SIZE_OF_SLICE, "foobar".len());
}

