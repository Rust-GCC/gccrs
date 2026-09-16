trait Foo: Iterator<Item = i32, Item = i32> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }

type Unit = ();

fn test() -> Box<dyn Iterator<Item = (), Item = Unit>> {
// { dg-error ".E0719." "" { target *-*-* } .-1 }
    Box::new(None.into_iter())
}

fn main() {
    let _: &dyn Iterator<Item = i32, Item = i32>;
    test();
}

