extern {
    fn f1(mut arg: u8); // { dg-error ".E0130." "" { target *-*-* } }
    fn f2(&arg: u8); // { dg-error ".E0130." "" { target *-*-* } }
    fn f3(arg @ _: u8); // { dg-error ".E0130." "" { target *-*-* } }
    fn g1(arg: u8); // OK
    fn g2(_: u8); // OK
    // fn g3(u8); // Not yet
}

type A1 = fn(mut arg: u8); // { dg-error ".E0561." "" { target *-*-* } }
type A2 = fn(&arg: u8); // { dg-error ".E0561." "" { target *-*-* } }
type B1 = fn(arg: u8); // OK
type B2 = fn(_: u8); // OK
type B3 = fn(u8); // OK

fn main() {}

