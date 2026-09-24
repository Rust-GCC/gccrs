// repr currently doesn't support literals
#[deprecated(since = b"1.29", note = "hi")] // { dg-error ".E0565." "" { target *-*-* } }
struct A {  }

fn main() {  }

