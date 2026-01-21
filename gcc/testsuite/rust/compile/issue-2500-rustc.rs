impl u8 {
    // { dg-error "cannot define inherent `impl` for primitive types" "" { target *-*-* } .-1 }
    pub const B: u8 = 0;
}

impl str {
    // { dg-error "cannot define inherent `impl` for primitive types" "" { target *-*-* } .-1 }
    fn foo() {}
}

impl char {
    // { dg-error "cannot define inherent `impl` for primitive types" "" { target *-*-* } .-1 }
    pub const B: u8 = 0;
    pub const C: u8 = 0;
    fn foo() {}
    fn bar(self) {}
}

struct MyType;
impl &MyType {
    // { dg-error "cannot define inherent `impl` for primitive types" "" { target *-*-* } .-1 }
    pub fn for_ref(self) {}
}

fn main() {}
