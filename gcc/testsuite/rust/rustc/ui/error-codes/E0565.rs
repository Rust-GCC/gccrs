// repr currently doesn't support literals
#[repr("C")] // { dg-error ".E0565." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
struct A {  }

fn main() {  }

