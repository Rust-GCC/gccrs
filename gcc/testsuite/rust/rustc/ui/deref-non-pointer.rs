fn main() {
  match *1 { // { dg-error ".E0614." "" { target *-*-* } }
      _ => { panic!(); }
  }
}

