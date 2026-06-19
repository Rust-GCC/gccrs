// this code used to cause an ICE

fn main() {
  let t = Err(0);
  match t {
    Some(k) => match k { // { dg-error ".E0308." "" { target *-*-* } }
      a => println!("{}", a)
    },
    None => () // { dg-error ".E0308." "" { target *-*-* } }
  }
}

