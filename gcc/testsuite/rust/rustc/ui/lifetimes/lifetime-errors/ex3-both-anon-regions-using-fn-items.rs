fn foo(x:fn(&u8, &u8), y: Vec<&u8>, z: &u8) {
  y.push(z); // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() { }

