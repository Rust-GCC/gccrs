unsafe fn f() { return; }

fn main() {
    let x = f;
    x();    // { dg-error ".E0133." "" { target *-*-* } }
}

