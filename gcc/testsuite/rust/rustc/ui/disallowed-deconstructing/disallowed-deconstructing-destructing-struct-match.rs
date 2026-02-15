struct X {
    x: String,
}

impl Drop for X {
    fn drop(&mut self) {
        println!("value: {}", self.x);
    }
}

fn main() {
    let x = X { x: "hello".to_string() };

    match x {
// { dg-error ".E0509." "" { target *-*-* } .-1 }
        X { x: y } => println!("contents: {}", y)
    }
}

