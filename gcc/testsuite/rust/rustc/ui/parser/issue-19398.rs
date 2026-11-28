trait T {
    extern "Rust" unsafe fn foo();
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

