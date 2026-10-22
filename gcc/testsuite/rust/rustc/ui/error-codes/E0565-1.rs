// deprecated doesn't currently support literals
#[deprecated("since")] // { dg-error ".E0565." "" { target *-*-* } }
fn f() {  }

fn main() {  }

