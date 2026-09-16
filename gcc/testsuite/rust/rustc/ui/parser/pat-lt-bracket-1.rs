fn main() {
  match 42 {
    x < 7 => (),
// { dg-error "" "" { target *-*-* } .-1 }
    _ => ()
  }
}

