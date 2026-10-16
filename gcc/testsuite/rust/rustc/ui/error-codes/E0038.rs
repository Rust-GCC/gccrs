trait Trait {
    fn foo(&self) -> Self;
}

fn call_foo(x: Box<dyn Trait>) {
// { dg-error ".E0038." "" { target *-*-* } .-1 }
    let y = x.foo();
}

fn main() {
}

