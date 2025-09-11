trait Get {
    type Value;
    fn get(&self) -> <Self as Get>::Value;
}

fn get<T:Get,U:Get>(x: T, y: U) -> Get::Value {}
// { dg-error ".E0223." "" { target *-*-* } .-1 }

trait Grab {
    type Value;
    fn grab(&self) -> Grab::Value;
// { dg-error ".E0223." "" { target *-*-* } .-1 }

    fn get(&self) -> Get::Value;
// { dg-error ".E0223." "" { target *-*-* } .-1 }
}

trait Bar {}

trait Foo where Foo::Assoc: Bar {
// { dg-error ".E0223." "" { target *-*-* } .-1 }
    type Assoc;
}

type X = std::ops::Deref::Target;
// { dg-error ".E0223." "" { target *-*-* } .-1 }

fn main() {
}

