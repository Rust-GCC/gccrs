struct Point { x: isize, y: isize }
struct Empty;

fn main() {
    let origin = Point { x: 0, y: 0 };
    origin.0;
// { dg-error ".E0609." "" { target *-*-* } .-1 }
    Empty.0;
// { dg-error ".E0609." "" { target *-*-* } .-1 }
}

