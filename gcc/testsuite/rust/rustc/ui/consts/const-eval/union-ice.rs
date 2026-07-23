#![feature(const_fn)]

type Field1 = i32;
type Field3 = i64;

#[repr(C)]
union DummyUnion {
    field1: Field1,
    field3: Field3,
}

const UNION: DummyUnion = DummyUnion { field1: 1065353216 };

const FIELD3: Field3 = unsafe { UNION.field3 }; // { dg-error ".E0080." "" { target *-*-* } }

const FIELD_PATH: Struct = Struct { // { dg-error ".E0080." "" { target *-*-* } }
    a: 42,
    b: unsafe { UNION.field3 },
};

struct Struct {
    a: u8,
    b: Field3,
}

const FIELD_PATH2: Struct2 = Struct2 { // { dg-error ".E0080." "" { target *-*-* } }
    b: [
        21,
        unsafe { UNION.field3 },
        23,
        24,
    ],
    a: 42,
};

struct Struct2 {
    b: [Field3; 4],
    a: u8,
}

fn main() {
}

