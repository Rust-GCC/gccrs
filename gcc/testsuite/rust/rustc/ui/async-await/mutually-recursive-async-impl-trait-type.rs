// edition:2018
// Test that impl trait does not allow creating recursive types that are
// otherwise forbidden when using `async` and `await`.

async fn rec_1() { // { dg-error ".E0733." "" { target *-*-* } }
    rec_2().await;
}

async fn rec_2() { // { dg-error ".E0733." "" { target *-*-* } }
    rec_1().await;
}

fn main() {}

