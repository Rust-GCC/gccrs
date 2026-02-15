// Check that an unsafe impl does not imply that unsafe actions are
// legal in the methods.

unsafe trait UnsafeTrait : Sized {
    fn foo(self) { }
}

unsafe impl UnsafeTrait for *mut isize {
    fn foo(self) {
        // Unsafe actions are not made legal by taking place in an unsafe trait:
        *self += 1;
// { dg-error ".E0133." "" { target *-*-* } .-1 }
    }
}

fn main() { }

