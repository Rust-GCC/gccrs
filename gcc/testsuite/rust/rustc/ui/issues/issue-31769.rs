fn main() {
    #[inline] struct Foo;  // { dg-error ".E0518." "" { target *-*-* } }
    #[repr(C)] fn foo() {} // { dg-error ".E0517." "" { target *-*-* } }
}

