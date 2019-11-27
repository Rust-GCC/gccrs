struct Point {
    x: f32,
    y: f32
}

struct Rectangle {
    p1: Point,
    p2: Point
}

// Function that returns a boolean value
fn is_divisible_by(lhs: u32, rhs: u32) -> bool {
    // Corner case, early return
    if rhs == 0 {
        return false;
    }

    // This is an expression, the `return` keyword is not necessary here
    return lhs % rhs == 0;
}



fn main() {
    let point: Point = Point { x: 0.3, y: 0.4 };
}

