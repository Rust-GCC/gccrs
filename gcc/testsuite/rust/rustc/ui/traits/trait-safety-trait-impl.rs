// Check that unsafe traits require unsafe impls and that inherent
// impls cannot be unsafe.

trait SafeTrait {
    fn foo(&self) { }
}

unsafe trait UnsafeTrait {
    fn foo(&self) { }
}

unsafe impl UnsafeTrait for u8 { } // OK

impl UnsafeTrait for u16 { } // { dg-error ".E0200." "" { target *-*-* } }

unsafe impl SafeTrait for u32 { } // { dg-error ".E0199." "" { target *-*-* } }

fn main() { }

