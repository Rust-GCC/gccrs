#[derive(Debug)]
enum Shape {
  Square { size: i32 },
  Circle { radius: i32 },
}

struct S {
  x: usize,
}

fn main() {
    println!("My shape is {:?}", Shape::Squareee { size: 5});  // { dg-error ".E0599." "" { target *-*-* } }
    println!("My shape is {:?}", Shape::Circl { size: 5}); // { dg-error ".E0599." "" { target *-*-* } }
    println!("My shape is {:?}", Shape::Rombus{ size: 5}); // { dg-error ".E0599." "" { target *-*-* } }
    Shape::Squareee; // { dg-error ".E0599." "" { target *-*-* } }
    Shape::Circl; // { dg-error ".E0599." "" { target *-*-* } }
    Shape::Rombus; // { dg-error ".E0599." "" { target *-*-* } }
}

