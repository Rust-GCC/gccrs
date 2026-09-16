fn a(x: String) -> String {
    format!("First function with {}", x)
}

fn a(x: String, y: String) -> String { // { dg-error ".E0428." "" { target *-*-* } }
    format!("Second function with {} and {}", x, y)
}

fn main() {
    println!("Result: ");
}

