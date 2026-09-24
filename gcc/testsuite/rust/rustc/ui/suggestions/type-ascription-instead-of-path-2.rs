// run-rustfix
fn main() -> Result<(), ()> {
    let _ = vec![Ok(2)].into_iter().collect:<Result<Vec<_>,_>>()?;
// { dg-error "" "" { target *-*-* } .-1 }
    Ok(())
}

