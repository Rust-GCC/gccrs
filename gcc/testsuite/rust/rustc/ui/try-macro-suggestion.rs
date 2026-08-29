// compile-flags: --edition 2018
fn foo() -> Result<(), ()> {
    Ok(try!()); // { dg-error "" "" { target *-*-* } }
    Ok(try!(Ok(()))) // { dg-error "" "" { target *-*-* } }
}

fn main() {
    let _ = foo();
}

