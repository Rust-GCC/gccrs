trait T1 {}
trait T2 {}

trait Foo {
    type A: T1;
}

trait Bar : Foo {
    type A: T2;
    fn do_something() {
        let _: Self::A;
// { dg-error ".E0221." "" { target *-*-* } .-1 }
    }
}

trait T3 {}

trait My : std::str::FromStr {
    type Err: T3;
    fn test() {
        let _: Self::Err;
// { dg-error ".E0221." "" { target *-*-* } .-1 }
    }
}

fn main() {
}

