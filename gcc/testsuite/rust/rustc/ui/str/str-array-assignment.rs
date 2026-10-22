fn main() {
  let s = "abc";
  let t = if true { s[..2] } else { s };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
  let u: &str = if true { s[..2] } else { s };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
  let v = s[..2];
// { dg-error ".E0277." "" { target *-*-* } .-1 }
  let w: &str = s[..2];
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

