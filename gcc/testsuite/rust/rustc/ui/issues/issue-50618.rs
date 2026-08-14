struct Point {
    pub x: u64,
    pub y: u64,
}

const TEMPLATE: Point = Point {
    x: 0,
    y: 0
};

fn main() {
    let _ = || {
        Point {
            nonexistent: 0,
// { dg-error ".E0560." "" { target *-*-* } .-1 }
            ..TEMPLATE
        }
    };
}

