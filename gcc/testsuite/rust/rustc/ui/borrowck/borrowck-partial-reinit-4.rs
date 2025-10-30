struct Test;

struct Test2(Option<Test>);

impl Drop for Test {
    fn drop(&mut self) {
        println!("dropping!");
    }
}

impl Drop for Test2 {
    fn drop(&mut self) {}
}

fn stuff() {
    let mut x : (Test2, Test2);
    (x.0).0 = Some(Test);
// { dg-error ".E0381." "" { target *-*-* } .-1 }
}

fn main() {
    stuff()
}

