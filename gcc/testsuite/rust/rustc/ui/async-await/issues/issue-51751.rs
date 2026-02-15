// edition:2018

async fn inc(limit: i64) -> i64 {
    limit + 1
}

fn main() {
    let result = inc(10000);
    let finished = result.await;
// { dg-error ".E0728." "" { target *-*-* } .-1 }
}

