#![deny(unused_variables)]

struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let points = vec!(Point { x: 1, y: 2 }, Point { x: 3, y: 4 });

    let _: i32 = points.iter()
        .map(|Point { x, y }| y)
// { dg-error "" "" { target *-*-* } .-1 }
        .sum();

    let _: i32 = points.iter()
        .map(|x| 4)
// { dg-error "" "" { target *-*-* } .-1 }
        .sum();
}

