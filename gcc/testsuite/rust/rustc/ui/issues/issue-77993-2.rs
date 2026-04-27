// edition:2018

async fn test() -> Result<(), Box<dyn std::error::Error>> {
    macro!();
// { dg-error "" "" { target *-*-* } .-1 }
    Ok(())
}

fn main() {}

