#[derive(Debug)]
struct Foo {
  i: isize,
}

impl Drop for Foo {
    fn drop(&mut self) {}
}

fn foo(i:isize) -> Foo {
    Foo {
        i: i
    }
}

fn main() {
    let x = foo(10);
    let _y = x.clone();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    println!("{:?}", x);
}

