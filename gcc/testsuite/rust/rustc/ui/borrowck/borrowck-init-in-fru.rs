#[derive(Clone)]
struct Point {
    x: isize,
    y: isize,
}

fn main() {
    let mut origin: Point;
    origin = Point { x: 10, ..origin };
// { dg-error ".E0381." "" { target *-*-* } .-1 }
    origin.clone();
}

