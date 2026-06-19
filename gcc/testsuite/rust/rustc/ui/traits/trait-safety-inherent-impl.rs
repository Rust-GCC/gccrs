// Check that inherent impls cannot be unsafe.

struct SomeStruct;

unsafe impl SomeStruct { // { dg-error ".E0197." "" { target *-*-* } }
    fn foo(self) { }
}

fn main() { }

