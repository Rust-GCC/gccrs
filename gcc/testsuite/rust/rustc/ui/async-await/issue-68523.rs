// edition:2018

async fn main() -> Result<i32, ()> {
// { dg-error ".E0752." "" { target *-*-* } .-1 }
// { dg-error ".E0752." "" { target *-*-* } .-2 }
    Ok(1)
}

