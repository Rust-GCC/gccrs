#[path = "circular_modules_hello.rs"]
mod circular_modules_hello; // { dg-error "" "" { target *-*-* } }

pub fn hi_str() -> String {
    "Hi!".to_string()
}

fn main() {
    circular_modules_hello::say_hello(); // { dg-error ".E0425." "" { target *-*-* } }
}

