struct X {
    x: String,
}

impl Drop for X {
    fn drop(&mut self) {
        println!("value: {}", self.x);
    }
}

fn unwrap(x: X) -> String {
    let X { x: y } = x; // { dg-error ".E0509." "" { target *-*-* } }
    y
}

fn main() {
    let x = X { x: "hello".to_string() };
    let y = unwrap(x);
    println!("contents: {}", y);
}

