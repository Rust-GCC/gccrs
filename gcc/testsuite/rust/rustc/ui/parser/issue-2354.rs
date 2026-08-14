fn foo() { // { dg-note "" "" { target *-*-* } }
  match Some(10) {
// { dg-note "" "" { target *-*-* } .-1 }
      Some(y) => { panic!(); }
      None => { panic!(); }
}
// { dg-note "" "" { target *-*-* } .-1 }

fn bar() {
    let mut i = 0;
    while (i < 1000) {}
}

fn main() {}
// { dg-error "" "" { target *-*-* } }

