// edition:2018

pub async fn f(x: Option<usize>) {
    x.take();
// { dg-error ".E0596." "" { target *-*-* } .-1 }
}

pub async fn g(x: usize) {
    x += 1;
// { dg-error ".E0384." "" { target *-*-* } .-1 }
}

fn main() {}

