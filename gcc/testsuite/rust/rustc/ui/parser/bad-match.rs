fn main() {
  let isize x = 5; // { dg-error "" "" { target *-*-* } }
  match x;
}

