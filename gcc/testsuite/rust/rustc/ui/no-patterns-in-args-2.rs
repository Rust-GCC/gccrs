#![deny(patterns_in_fns_without_body)]

trait Tr {
    fn f1(mut arg: u8); // { dg-error ".E0642." "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
    fn f2(&arg: u8); // { dg-error ".E0642." "" { target *-*-* } }
    fn g1(arg: u8); // OK
    fn g2(_: u8); // OK
    #[allow(anonymous_parameters)]
    fn g3(u8); // OK
}

fn main() {}

