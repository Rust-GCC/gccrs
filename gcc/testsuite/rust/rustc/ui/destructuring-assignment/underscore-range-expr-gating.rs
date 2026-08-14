fn main() {}

struct S { x : u32 }

#[cfg(FALSE)]
fn foo() {
    S { x: 5, .. }; // { dg-error ".E0658." "" { target *-*-* } }
}

