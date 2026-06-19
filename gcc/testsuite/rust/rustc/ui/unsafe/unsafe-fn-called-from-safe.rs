unsafe fn f() { return; }

fn main() {
    f(); // { dg-error ".E0133." "" { target *-*-* } }
}

