struct Foo;

impl Foo {
    fn bar(&self) {}
}

trait MyTrait {
    fn trait_bar() {}
}

impl MyTrait for Foo {}

fn main() {
    match 0u32 {
        Foo::bar => {}
// { dg-error ".E0533." "" { target *-*-* } .-1 }
    }
    match 0u32 {
        <Foo>::bar => {}
// { dg-error ".E0533." "" { target *-*-* } .-1 }
    }
    match 0u32 {
        <Foo>::trait_bar => {}
// { dg-error ".E0533." "" { target *-*-* } .-1 }
    }
    if let Foo::bar = 0u32 {}
// { dg-error ".E0533." "" { target *-*-* } .-1 }
    if let <Foo>::bar = 0u32 {}
// { dg-error ".E0533." "" { target *-*-* } .-1 }
    if let Foo::trait_bar = 0u32 {}
// { dg-error ".E0533." "" { target *-*-* } .-1 }
}

