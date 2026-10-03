// Regression test for #65159. We used to ICE.
//
// edition:2018

async fn copy() -> Result<()> // { dg-error ".E0107." "" { target *-*-* } }
{
    Ok(())
// { dg-error ".E0282." "" { target *-*-* } .-1 }
}

fn main() { }

