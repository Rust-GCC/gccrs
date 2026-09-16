fn forever2() -> i32 {
  let x: i32 = loop { break }; // { dg-error ".E0308." "" { target *-*-* } }
  x
}

fn main() {}

