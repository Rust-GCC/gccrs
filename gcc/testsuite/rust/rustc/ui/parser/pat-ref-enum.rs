fn matcher(x: Option<isize>) {
    match x {
      ref Some(i) => {} // { dg-error "" "" { target *-*-* } }
      None => {}
    }
}

fn main() {}

