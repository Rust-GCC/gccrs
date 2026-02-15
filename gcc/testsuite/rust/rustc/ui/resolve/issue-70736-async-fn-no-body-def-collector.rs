// edition:2018

async fn free(); // { dg-error "" "" { target *-*-* } }

struct A;
impl A {
    async fn inherent(); // { dg-error "" "" { target *-*-* } }
}

trait B {
    async fn associated();
// { dg-error ".E0706." "" { target *-*-* } .-1 }
}
impl B for A {
    async fn associated(); // { dg-error ".E0053." "" { target *-*-* } }
// { dg-error ".E0053." "" { target *-*-* } .-1 }
// { dg-error ".E0053." "" { target *-*-* } .-2 }
}

fn main() {}

