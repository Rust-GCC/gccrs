fn foo() {
  let s = "abc";
  let u: &str = if true { s[..2] } else { s };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn main() {
    foo();
}

