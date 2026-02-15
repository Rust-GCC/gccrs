struct Point {
    x: f64,
    y: f64,
}

trait ToString_ {
    fn to_string(&self) -> String;
}

impl ToString_ for Point {
    fn new(x: f64, y: f64) -> Point {
// { dg-error ".E0407." "" { target *-*-* } .-1 }
        Point { x: x, y: y }
    }

    fn to_string(&self) -> String {
        format!("({}, {})", self.x, self.y)
    }
}

fn main() {
    let p = Point::new(0.0, 0.0);
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    println!("{}", p.to_string());
}

