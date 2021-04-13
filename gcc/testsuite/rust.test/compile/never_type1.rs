fn foo(x: i32) -> f32 {
    let y = if x > 1 {
        return 2.0;
    } else {
        1
    };

    let z = if x < -1 {
        -1
    } else {
        return -2.0;
    };

    let mut w: (f32, i32, f32) = (2.3, y + z, 3.3);
    w = (1.0, return 0.0, -1.0);
    w = (6.6, 66, 6.6); // { dg-warning "unreachable statement" }
}

fn main() {
    foo(-2);
    foo(0);
    foo(2);
}
