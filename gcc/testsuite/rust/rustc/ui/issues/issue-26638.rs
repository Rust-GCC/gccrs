fn parse_type(iter: Box<dyn Iterator<Item=&str>+'static>) -> &str { iter.next() }
// { dg-error ".E0106." "" { target *-*-* } .-1 }

fn parse_type_2(iter: fn(&u8)->&u8) -> &str { iter() }
// { dg-error ".E0106." "" { target *-*-* } .-1 }

fn parse_type_3() -> &str { unimplemented!() }
// { dg-error ".E0106." "" { target *-*-* } .-1 }

fn main() {}

