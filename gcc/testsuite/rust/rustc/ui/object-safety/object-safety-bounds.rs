// Traits with bounds mentioning `Self` are not object safe

trait X {
    type U: PartialEq<Self>;
}

fn f() -> Box<dyn X<U = u32>> {
// { dg-error ".E0038." "" { target *-*-* } .-1 }
    loop {}
}

fn main() {}

