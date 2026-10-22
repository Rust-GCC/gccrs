struct MyStruct;

impl Drop for MyStruct {
    fn drop(&mut self) { }
}

impl Drop for MyStruct {
// { dg-error ".E0119." "" { target *-*-* } .-1 }
    fn drop(&mut self) { }
}

fn main() {}

