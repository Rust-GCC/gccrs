fn foo(a: Option<usize>, b: Option<usize>) {
  match (a,b) {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    (Some(a), Some(b)) if a == b => { }
    (Some(_), None) |
    (None, Some(_)) => { }
  }
}

fn main() {
  foo(None, None);
}

