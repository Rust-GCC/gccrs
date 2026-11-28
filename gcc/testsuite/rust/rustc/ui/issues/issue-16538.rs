mod Y {
    pub type X = usize;
    extern {
        pub static x: *const usize;
    }
    pub fn foo(value: *const X) -> *const X {
        value
    }
}

static foo: *const Y::X = Y::foo(Y::x as *const Y::X);
// { dg-error ".E0133." "" { target *-*-* } .-1 }
// { dg-error ".E0133." "" { target *-*-* } .-2 }
// { dg-error ".E0133." "" { target *-*-* } .-3 }

fn main() {}

