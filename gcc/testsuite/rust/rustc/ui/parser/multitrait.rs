struct S {
 y: isize
}

impl Cmp, ToString for S {
// { dg-error "" "" { target *-*-* } .-1 }
  fn eq(&&other: S) { false }
  fn to_string(&self) -> String { "hi".to_string() }
}

