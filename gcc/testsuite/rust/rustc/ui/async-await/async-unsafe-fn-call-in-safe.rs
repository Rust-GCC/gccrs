// edition:2018

struct S;

impl S {
    async unsafe fn f() {}
}

async unsafe fn f() {}

async fn g() {
    S::f(); // { dg-error ".E0133." "" { target *-*-* } }
    f(); // { dg-error ".E0133." "" { target *-*-* } }
}

fn main() {
    S::f(); // { dg-error ".E0133." "" { target *-*-* } }
    f(); // { dg-error ".E0133." "" { target *-*-* } }
}

