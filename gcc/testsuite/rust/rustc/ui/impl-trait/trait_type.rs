struct MyType;
struct MyType2;
struct MyType3;
struct MyType4;

impl std::fmt::Display for MyType {
   fn fmt(&self, x: &str) -> () { }
// { dg-error ".E0053." "" { target *-*-* } .-1 }
}

impl std::fmt::Display for MyType2 {
   fn fmt(&self) -> () { }
// { dg-error ".E0050." "" { target *-*-* } .-1 }
}

impl std::fmt::Display for MyType3 {
   fn fmt() -> () { }
// { dg-error ".E0186." "" { target *-*-* } .-1 }
}

impl std::fmt::Display for MyType4 {}
// { dg-error ".E0046." "" { target *-*-* } .-1 }

fn main() {}

