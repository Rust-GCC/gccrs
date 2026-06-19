#[derive(Debug)]
struct R {
  b: bool,
}

impl Drop for R {
    fn drop(&mut self) {}
}

fn main() {
    let i = Box::new(R { b: true });
    let _j = i.clone(); // { dg-error ".E0599." "" { target *-*-* } }
    println!("{:?}", i);
}

